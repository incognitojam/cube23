#pragma once

#include <string>

namespace Vox {
    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;

        static Shader *create(const std::string &vertexSrc, const std::string &fragmentSrc);
    };
}
