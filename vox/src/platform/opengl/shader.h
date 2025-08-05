#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

#include "vox/renderer/shader.h"

typedef unsigned int GLenum;

namespace Vox {
    class OpenGLShader final : public Shader {
    public:
        explicit OpenGLShader(const std::string &filepath);
        OpenGLShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
        ~OpenGLShader() override;

        void bind() override;
        void unbind() override;

        const std::string &getName() const override { return mName; }

        void setInt(const std::string &name, int value) override;
        void setFloat(const std::string &name, float value) override;
        void setFloat2(const std::string &name, const glm::vec2 &value) override;
        void setFloat3(const std::string &name, const glm::vec3 &value) override;
        void setFloat4(const std::string &name, const glm::vec4 &value) override;
        void setMat3(const std::string &name, const glm::mat3 &matrix) override;
        void setMat4(const std::string &name, const glm::mat4 &matrix) override;

    private:
        static std::string readFile(const std::string &filepath);
        static std::unordered_map<GLenum, std::string> preprocess(const std::string &source);
        void compile(const std::unordered_map<GLenum, std::string> &shaderSources);

        std::string mName;
        uint32_t mRendererID;
    };
}
