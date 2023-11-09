#pragma once

#include "vox/renderer/graphics_context.h"

struct GLFWwindow;

namespace Vox {
    class OpenGLContext : public GraphicsContext {
    public:
        OpenGLContext(GLFWwindow *windowHandle);

        virtual void init() override;
        virtual void swapBuffers() override;

    private:
        GLFWwindow *mWindowHandle;
    };
}
