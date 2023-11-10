#pragma once

#include <cstdint>
#include <memory>

namespace Vox {
    class Texture {
    public:
        virtual ~Texture() = default;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual void bind(uint32_t slot) const = 0;
    };

    class Texture2D : public Texture {
    public:
        static std::shared_ptr<Texture2D> create(const std::string &path);
    };
}
