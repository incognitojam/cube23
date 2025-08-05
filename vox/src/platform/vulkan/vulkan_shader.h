#pragma once

#include "vox/renderer/shader.h"
#include <glm/glm.hpp>

namespace Vox {
    class VulkanShader : public Shader {
    public:
        VulkanShader(const std::string &filepath);
        VulkanShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
        virtual ~VulkanShader();

        void bind() override;
        void unbind() override;

        void setInt(const std::string &name, int value) override;
        void setFloat(const std::string &name, float value) override;
        void setFloat2(const std::string &name, const glm::vec2 &value) override;
        void setFloat3(const std::string &name, const glm::vec3 &value) override;
        void setFloat4(const std::string &name, const glm::vec4 &value) override;
        void setMat3(const std::string &name, const glm::mat3 &matrix) override;
        void setMat4(const std::string &name, const glm::mat4 &matrix) override;

        const std::string &getName() const override { return mName; }

    private:
        void readFile(const std::string &filepath);
        void compileOrGetVulkanBinaries();
        void createShaderModules();

    private:
        std::string mName;
        // TODO: Add Vulkan shader handles
    };
}
