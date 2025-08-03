#pragma once

#include "vox/renderer/texture.h"
#include <string>

namespace Vox {
    class VulkanTexture2D : public Texture2D {
    public:
        VulkanTexture2D(uint32_t width, uint32_t height);
        VulkanTexture2D(const std::string &path);
        virtual ~VulkanTexture2D();

        uint32_t getWidth() const override { return mWidth; }
        uint32_t getHeight() const override { return mHeight; }

        void setData(void *data, uint32_t size);

        void bind(uint32_t slot = 0) const override;

        bool operator==(const Texture &other) const {
            return mTextureID == ((VulkanTexture2D&)other).mTextureID;
        }

    private:
        std::string mPath;
        uint32_t mWidth, mHeight;
        uint32_t mTextureID; // Temporary ID for comparison
        // TODO: Add Vulkan image handles
    };
}
