#pragma once

#include "vox/renderer/vertex_array.h"

namespace Vox {
    class VulkanVertexArray : public VertexArray {
    public:
        VulkanVertexArray();
        virtual ~VulkanVertexArray();

        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
        void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

        const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const override { return mVertexBuffers; }
        const std::shared_ptr<IndexBuffer> &getIndexBuffer() const override { return mIndexBuffer; }

    private:
        std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
        std::shared_ptr<IndexBuffer> mIndexBuffer;
        // TODO: Add Vulkan pipeline state tracking
    };
}
