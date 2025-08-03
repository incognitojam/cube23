#include "vulkan_shader.h"

namespace Vox {
    VulkanShader::VulkanShader(const std::string &filepath) {
        readFile(filepath);
        
        // Extract name from filepath
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        mName = filepath.substr(lastSlash, count);

        compileOrGetVulkanBinaries();
        createShaderModules();
    }

    VulkanShader::VulkanShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
        : mName(name) {
        // TODO: Handle source string compilation for Vulkan
        compileOrGetVulkanBinaries();
        createShaderModules();
    }

    VulkanShader::~VulkanShader() {
        // TODO: Cleanup Vulkan shader modules
    }

    void VulkanShader::bind() {
        // TODO: Bind Vulkan shader (set active pipeline)
    }

    void VulkanShader::unbind() {
        // TODO: Unbind Vulkan shader
    }

    void VulkanShader::setInt(const std::string &name, int value) {
        // TODO: Set Vulkan uniform via descriptor sets
    }

    void VulkanShader::setFloat(const std::string &name, float value) {
        // TODO: Set Vulkan uniform via descriptor sets
    }

    void VulkanShader::setFloat2(const std::string &name, const glm::vec2 &value) {
        // TODO: Set Vulkan uniform via descriptor sets
    }

    void VulkanShader::setFloat3(const std::string &name, const glm::vec3 &value) {
        // TODO: Set Vulkan uniform via descriptor sets
    }

    void VulkanShader::setFloat4(const std::string &name, const glm::vec4 &value) {
        // TODO: Set Vulkan uniform via descriptor sets
    }

    void VulkanShader::setMat3(const std::string &name, const glm::mat3 &matrix) {
        // TODO: Set Vulkan uniform via descriptor sets
    }

    void VulkanShader::setMat4(const std::string &name, const glm::mat4 &matrix) {
        // TODO: Set Vulkan uniform via descriptor sets
    }

    void VulkanShader::readFile(const std::string &filepath) {
        // TODO: Read SPIR-V binary file for Vulkan
    }

    void VulkanShader::compileOrGetVulkanBinaries() {
        // TODO: Load pre-compiled SPIR-V binaries
    }

    void VulkanShader::createShaderModules() {
        // TODO: Create VkShaderModule objects
    }
}
