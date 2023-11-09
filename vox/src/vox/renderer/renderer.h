#pragma once

namespace Vox {
    enum class RendererAPI {
        None = 0,
        OpenGL = 1,
    };

    class Renderer {
    public:
        inline static RendererAPI getApi() { return sRendererApi; }

    private:
        static RendererAPI sRendererApi;
    };
}
