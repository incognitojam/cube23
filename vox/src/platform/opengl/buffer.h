#pragma once

#include "vox/renderer/buffer.h"

namespace Vox {
    class OpenGLVertexBuffer : public VertexBuffer {
    public:
        OpenGLVertexBuffer(const float *vertices, uint32_t size);
        ~OpenGLVertexBuffer() override;

        void bind() const override;
        void unbind() const override;

        const BufferLayout &getLayout() const override { return mLayout; }
        void setLayout(const BufferLayout &layout) override { mLayout = layout; }

    private:
        uint32_t mRendererID;
        BufferLayout mLayout;
    };

    class OpenGLIndexBuffer : public IndexBuffer {
    public:
        OpenGLIndexBuffer(const uint32_t *indices, uint32_t count);
        ~OpenGLIndexBuffer() override;

        void bind() const override;
        void unbind() const override;

        uint32_t getCount() const override { return mCount; }

    private:
        uint32_t mRendererID;
        uint32_t mCount;
    };
}
