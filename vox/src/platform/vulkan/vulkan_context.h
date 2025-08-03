#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>
#include <set>

#include "vox/renderer/graphics_context.h"

namespace Vox {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class VulkanContext : public GraphicsContext {
    public:
        VulkanContext(GLFWwindow* window);
        ~VulkanContext() override;

        void init() override;
        void swapBuffers() override;

        // Vulkan-specific getters
        VkInstance getInstance() const { return mInstance; }
        VkDevice getDevice() const { return mDevice; }
        VkPhysicalDevice getPhysicalDevice() const { return mPhysicalDevice; }
        VkQueue getGraphicsQueue() const { return mGraphicsQueue; }
        VkQueue getPresentQueue() const { return mPresentQueue; }
        VkSurfaceKHR getSurface() const { return mSurface; }
        VkCommandPool getCommandPool() const { return mCommandPool; }

        // Swapchain management
        VkSwapchainKHR getSwapchain() const { return mSwapchain; }
        VkFormat getSwapchainImageFormat() const { return mSwapchainImageFormat; }
        VkExtent2D getSwapchainExtent() const { return mSwapchainExtent; }
        const std::vector<VkImageView>& getSwapchainImageViews() const { return mSwapchainImageViews; }

        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device);

    private:
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapchain();
        void createImageViews();
        void createCommandPool();

        // Validation and debug
        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        
        // Device suitability
        bool isDeviceSuitable(VkPhysicalDevice device);
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);

        // Swapchain helpers
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        // Cleanup
        void cleanup();

    private:
        GLFWwindow* mWindow;

        // Core Vulkan objects
        VkInstance mInstance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;
        VkSurfaceKHR mSurface = VK_NULL_HANDLE;
        VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
        VkDevice mDevice = VK_NULL_HANDLE;

        // Queues
        VkQueue mGraphicsQueue = VK_NULL_HANDLE;
        VkQueue mPresentQueue = VK_NULL_HANDLE;

        // Swapchain
        VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
        std::vector<VkImage> mSwapchainImages;
        VkFormat mSwapchainImageFormat;
        VkExtent2D mSwapchainExtent;
        std::vector<VkImageView> mSwapchainImageViews;

        // Command pool
        VkCommandPool mCommandPool = VK_NULL_HANDLE;

        // Constants
        const std::vector<const char*> mValidationLayers = {
            "VK_LAYER_KHRONOS_validation",
        };

        const std::vector<const char*> mDeviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        };

#ifdef NDEBUG
        const bool mEnableValidationLayers = false;
#else
        const bool mEnableValidationLayers = true;
#endif
    };

    // Debug messenger helper functions
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks* pAllocator);

    // Debug callback
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                  void* pUserData);
}
