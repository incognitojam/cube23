#pragma once

#include "vox/renderer/vertex_array.h"

namespace Vox {
    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        ~OpenGLVertexArray() override;

        void bind() const override;
        void unbind() const override;

        void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) override;
        void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) override;

        const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const override { return mVertexBuffers; }
        const std::shared_ptr<IndexBuffer> &getIndexBuffer() const override { return mIndexBuffer; }

    private:
        uint32_t mRendererID;
        std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
        std::shared_ptr<IndexBuffer> mIndexBuffer;
    };
}
