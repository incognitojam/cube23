#pragma once

#include "vox/renderer/buffer.h"

namespace Vox {
    class VulkanVertexBuffer : public VertexBuffer {
    public:
        VulkanVertexBuffer(float *vertices, uint32_t size);
        virtual ~VulkanVertexBuffer();

        void bind() const override;
        void unbind() const override;

        const BufferLayout &getLayout() const override { return mLayout; }
        void setLayout(const BufferLayout &layout) override { mLayout = layout; }

    private:
        BufferLayout mLayout;
        // TODO: Add Vulkan buffer handles
    };

    class VulkanIndexBuffer : public IndexBuffer {
    public:
        VulkanIndexBuffer(uint32_t *indices, uint32_t count);
        virtual ~VulkanIndexBuffer();

        void bind() const override;
        void unbind() const override;

        uint32_t getCount() const override { return mCount; }

    private:
        uint32_t mCount;
        // TODO: Add Vulkan buffer handles
    };
}
