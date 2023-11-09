#pragma once

#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Vox {
    enum class ShaderDataType {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float: return 4;
            case ShaderDataType::Float2: return 4 * 2;
            case ShaderDataType::Float3: return 4 * 3;
            case ShaderDataType::Float4: return 4 * 4;
            case ShaderDataType::Mat3: return 4 * 3 * 3;
            case ShaderDataType::Mat4: return 4 * 4 * 4;
            case ShaderDataType::Int: return 4;
            case ShaderDataType::Int2: return 4 * 2;
            case ShaderDataType::Int3: return 4 * 3;
            case ShaderDataType::Int4: return 4 * 4;
            case ShaderDataType::Bool: return 1;
            default:
                throw std::runtime_error("Unknown ShaderDataType!");
        }
    }

    struct BufferElement {
        std::string name;
        ShaderDataType type;
        uint32_t size;
        uint32_t offset;
        bool normalized;

        BufferElement(ShaderDataType type, const std::string &name, bool normalized = false) : type(type),
            name(name), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized) {
        }

        uint32_t getComponentCount() const {
            switch (type) {
                case ShaderDataType::Float: return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3: return 3 * 3;
                case ShaderDataType::Mat4: return 4 * 4;
                case ShaderDataType::Int: return 1;
                case ShaderDataType::Int2: return 2;
                case ShaderDataType::Int3: return 3;
                case ShaderDataType::Int4: return 4;
                case ShaderDataType::Bool: return 1;
                default:
                    throw std::runtime_error("Unknown ShaderDataType!");
            }
        }
    };

    class BufferLayout {
    public:
        BufferLayout() = default;

        BufferLayout(const std::initializer_list<BufferElement> &elements) : mElements(elements) {
            calculateOffsetsAndStride();
        }

        inline uint32_t getStride() const { return mStride; }
        inline const std::vector<BufferElement> &getElements() const { return mElements; }

        std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
        std::vector<BufferElement>::iterator end() { return mElements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

    private:
        void calculateOffsetsAndStride() {
            uint32_t offset = 0;
            mStride = 0;
            for (auto &element : mElements) {
                element.offset = offset;
                offset += element.size;
                mStride += element.size;
            }
        }

    private:
        std::vector<BufferElement> mElements;
        uint32_t mStride = 0;
    };

    class VertexBuffer {
    public:
        virtual ~VertexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual const BufferLayout &getLayout() const = 0;
        virtual void setLayout(const BufferLayout &) = 0;

        static VertexBuffer *create(float *vertices, uint32_t size);
    };

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() {}

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t getCount() const = 0;

        static IndexBuffer *create(uint32_t *indices, uint32_t count);
    };
}
