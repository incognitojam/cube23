#include "platform/opengl/buffer.h"

#include <glad/glad.h>

namespace Vox {
    OpenGLVertexBuffer::OpenGLVertexBuffer(const float *vertices, const uint32_t size) {
        glGenBuffers(1, &mRendererID);
        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        glDeleteBuffers(1, &mRendererID);
    }

    void OpenGLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
    }

    void OpenGLVertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t *indices, const uint32_t count) : mCount(count) {
        glGenBuffers(1, &mRendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        glDeleteBuffers(1, &mRendererID);
    }

    void OpenGLIndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
    }

    void OpenGLIndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) : mBinding(binding) {
        glGenBuffers(1, &mRendererID);
        glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, mRendererID);
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer() {
        glDeleteBuffers(1, &mRendererID);
    }

    void OpenGLUniformBuffer::bind() const {
        glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
    }

    void OpenGLUniformBuffer::unbind() const {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void OpenGLUniformBuffer::setData(const void* data, uint32_t size, uint32_t offset) {
        glBindBuffer(GL_UNIFORM_BUFFER, mRendererID);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    }
}
