#include "platform/opengl/shader.h"

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

namespace Vox {
    static GLenum getShaderTypeFromString(const std::string &type) {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;
        throw std::runtime_error("Unknown shader type!");
    }

    OpenGLShader::OpenGLShader(const std::string &filepath) {
        const std::string source = readFile(filepath);
        auto shaderSources = preprocess(source);
        compile(shaderSources);

        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        mName = filepath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string &name, const std::string &vertexSrc,
                               const std::string &fragmentSrc) : mName(name) {
        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;
        compile(sources);
    }

    OpenGLShader::OpenGLShader(const std::string &name, const std::string &vertexSpvPath, 
                               const std::string &fragmentSpvPath, bool isSpirv) : mName(name) {
        if (isSpirv) {
            std::unordered_map<GLenum, std::vector<uint32_t>> spirvSources;
            spirvSources[GL_VERTEX_SHADER] = readSpirv(vertexSpvPath);
            spirvSources[GL_FRAGMENT_SHADER] = readSpirv(fragmentSpvPath);
            compileSpirv(spirvSources);
            reflectSpirv(spirvSources[GL_VERTEX_SHADER], spirvSources[GL_FRAGMENT_SHADER]);
        }
    }

    OpenGLShader::~OpenGLShader() {
        glDeleteProgram(mRendererID);
    }

    std::string OpenGLShader::readFile(const std::string &filepath) {
        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (!in) {
            throw std::runtime_error("Could not open file '" + filepath + "'!");
        }
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
        return result;
    }

    std::vector<uint32_t> OpenGLShader::readSpirv(const std::string &filepath) {
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open SPIR-V file '" + filepath + "'!");
        }

        size_t fileSize = file.tellg();
        std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
        
        file.seekg(0);
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
        file.close();
        
        return buffer;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preprocess(const std::string &source) {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char *typeToken = "#type";
        const size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos) {
            size_t eol = source.find_first_of("\r\n", pos);
            if (eol == std::string::npos) {
                throw std::runtime_error("Syntax error!");
            }
            size_t begin = pos + typeTokenLength + 1;
            GLenum type = getShaderTypeFromString(source.substr(begin, eol - begin));
            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            shaderSources[type] = source.substr(nextLinePos,
                                                pos - (nextLinePos == std::string::npos
                                                           ? source.size() - 1
                                                           : nextLinePos));
        }
        return shaderSources;
    }

    void OpenGLShader::compile(const std::unordered_map<GLenum, std::string> &shaderSources) {
        if (shaderSources.size() > 2) {
            throw std::runtime_error("Only 2 shaders are supported for now!");
        }
        std::array<GLenum, 2> glShaderIDs;
        int glShaderIDIndex = 0;

        mRendererID = glCreateProgram();

        for (const auto &[type, source] : shaderSources) {
            const GLuint shader = glCreateShader(type);

            const GLchar *sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);

            glCompileShader(shader);

            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (compiled == GL_FALSE) {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // TODO: log this error
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                throw std::runtime_error("Shader compilation failure!");
            }

            glAttachShader(mRendererID, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        glLinkProgram(mRendererID);

        GLint linked = 0;
        glGetProgramiv(mRendererID, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(mRendererID, GL_INFO_LOG_LENGTH, &maxLength);

            // TODO: log this error
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(mRendererID, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(mRendererID);
            for (const auto id : glShaderIDs)
                glDeleteShader(id);

            throw std::runtime_error("Shader link failure!");
        }

        for (const auto id : glShaderIDs)
            glDetachShader(mRendererID, id);

        // Set uniform block bindings for OpenGL 3.3 compatibility
        GLuint cameraBlockIndex = glGetUniformBlockIndex(mRendererID, "CameraData");
        if (cameraBlockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(mRendererID, cameraBlockIndex, 0);
        }
        
        GLuint objectBlockIndex = glGetUniformBlockIndex(mRendererID, "ObjectData");
        if (objectBlockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(mRendererID, objectBlockIndex, 1);
        }
    }

    void OpenGLShader::bind() {
        glUseProgram(mRendererID);
    }

    void OpenGLShader::unbind() {
        glUseProgram(0);
    }

    void OpenGLShader::setInt(const std::string &name, int value) {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::setFloat(const std::string &name, float value) {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::setFloat2(const std::string &name, const glm::vec2 &value) {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::setFloat3(const std::string &name, const glm::vec3 &value) {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::setFloat4(const std::string &name, const glm::vec4 &value) {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::setMat3(const std::string &name, const glm::mat3 &matrix) {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(matrix));
    }

    void OpenGLShader::setMat4(const std::string &name, const glm::mat4 &matrix) {
        const GLint location = glGetUniformLocation(mRendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
    }

    void OpenGLShader::compileSpirv(const std::unordered_map<GLenum, std::vector<uint32_t>> &shaderSources) {
        if (shaderSources.size() > 2) {
            throw std::runtime_error("Only 2 shaders are supported for now!");
        }

        // Convert SPIR-V to GLSL using spirv-cross
        std::unordered_map<GLenum, std::string> glslSources;
        
        for (const auto &[type, spirv] : shaderSources) {
            spirv_cross::CompilerGLSL glsl(spirv);
            spirv_cross::CompilerGLSL::Options options;
            options.version = 330;
            options.es = false;
            glsl.set_common_options(options);
            
            glslSources[type] = glsl.compile();
        }
        
        // Use existing compile method with converted GLSL
        compile(glslSources);
    }

    void OpenGLShader::reflectSpirv(const std::vector<uint32_t> &vertexSpirv, const std::vector<uint32_t> &fragmentSpirv) {
        // Reflect vertex shader
        spirv_cross::Compiler vertexCompiler(vertexSpirv);
        spirv_cross::ShaderResources vertexResources = vertexCompiler.get_shader_resources();
        
        // Process uniform buffers
        for (const auto &resource : vertexResources.uniform_buffers) {
            ShaderUniformBuffer buffer;
            buffer.name = resource.name;
            buffer.binding = vertexCompiler.get_decoration(resource.id, spv::DecorationBinding);
            
            const spirv_cross::SPIRType &type = vertexCompiler.get_type(resource.base_type_id);
            buffer.size = vertexCompiler.get_declared_struct_size(type);
            
            // Get members of the uniform buffer
            for (uint32_t i = 0; i < type.member_types.size(); ++i) {
                ShaderUniform uniform;
                uniform.name = vertexCompiler.get_member_name(resource.base_type_id, i);
                uniform.offset = vertexCompiler.type_struct_member_offset(type, i);
                
                const spirv_cross::SPIRType &memberType = vertexCompiler.get_type(type.member_types[i]);
                uniform.size = vertexCompiler.get_declared_struct_member_size(type, i);
                
                // Map SPIR-V types to our enum
                if (memberType.basetype == spirv_cross::SPIRType::Float) {
                    if (memberType.vecsize == 1) uniform.type = ShaderUniformType::Float;
                    else if (memberType.vecsize == 2) uniform.type = ShaderUniformType::Float2;
                    else if (memberType.vecsize == 3) uniform.type = ShaderUniformType::Float3;
                    else if (memberType.vecsize == 4) uniform.type = ShaderUniformType::Float4;
                    else if (memberType.columns == 4 && memberType.vecsize == 4) uniform.type = ShaderUniformType::Mat4;
                    else if (memberType.columns == 3 && memberType.vecsize == 3) uniform.type = ShaderUniformType::Mat3;
                }
                
                buffer.uniforms.push_back(uniform);
            }
            
            mUniformBuffers.push_back(buffer);
        }
        
        // Process individual uniforms (like samplers)
        for (const auto &resource : vertexResources.sampled_images) {
            ShaderUniform uniform;
            uniform.name = resource.name;
            uniform.type = ShaderUniformType::Sampler2D;
            uniform.location = vertexCompiler.get_decoration(resource.id, spv::DecorationBinding);
            mUniforms.push_back(uniform);
        }
        
        // Also check fragment shader for samplers
        spirv_cross::Compiler fragmentCompiler(fragmentSpirv);
        spirv_cross::ShaderResources fragmentResources = fragmentCompiler.get_shader_resources();
        
        for (const auto &resource : fragmentResources.sampled_images) {
            ShaderUniform uniform;
            uniform.name = resource.name;
            uniform.type = ShaderUniformType::Sampler2D;
            uniform.location = fragmentCompiler.get_decoration(resource.id, spv::DecorationBinding);
            mUniforms.push_back(uniform);
        }
    }
}
