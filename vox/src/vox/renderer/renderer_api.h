#pragma once

#include <glm/glm.hpp>
#include <cstdlib>
#include <cstring>

#include "vox/renderer/vertex_array.h"

namespace Vox {
    class RendererAPI {
    public:
        enum class API {
            None = 0,
            OpenGL = 1,
            Vulkan = 2,
        };

        virtual void init() = 0;

        virtual void setClearColor(const glm::vec4 &color) = 0;
        virtual void clear() = 0;

        virtual void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) = 0;

        static API getAPI() {
            static API api = []() {
                const char* renderer = std::getenv("VOX_RENDERER");
                if (renderer && strcmp(renderer, "vulkan") == 0) {
                    return API::Vulkan;
                }
                return API::OpenGL; // default
            }();
            return api;
        }
    };
}
