#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "vox/renderer/shader.h"

typedef unsigned int GLenum;

namespace Vox {
    class OpenGLShader final : public Shader {
    public:
        explicit OpenGLShader(const std::string &filepath);
        OpenGLShader(const std::string &vertexSrc, const std::string &fragmentSrc);
        ~OpenGLShader() override;

        void bind() override;
        void unbind() override;

        void uploadUniformInt(const std::string &name, int value) const;

        void uploadUniformFloat(const std::string &name, float value) const;
        void uploadUniformFloat2(const std::string &name, const glm::vec2 &value) const;
        void uploadUniformFloat3(const std::string &name, const glm::vec3 &value) const;
        void uploadUniformFloat4(const std::string &name, const glm::vec4 &value) const;

        void uploadUniformMat3(const std::string &name, const glm::mat3 &matrix) const;
        void uploadUniformMat4(const std::string &name, const glm::mat4 &matrix) const;

    private:
        static std::string readFile(const std::string &filepath);
        static std::unordered_map<GLenum, std::string> preprocess(const std::string &source);
        void compile(const std::unordered_map<GLenum, std::string> &shaderSources);

        uint32_t mRendererID;
    };
}
