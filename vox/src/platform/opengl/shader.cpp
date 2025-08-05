#include "platform/opengl/shader.h"

#include <array>
#include <fstream>
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

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
}
