#include "platform/opengl/opengl_context.h"

#include <stdexcept>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/gl.h>

namespace Vox {

    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle) : mWindowHandle(windowHandle) {
        if (mWindowHandle == nullptr) {
            throw std::runtime_error("Window handle is null!");
        }
    }

    void OpenGLContext::init() {
        glfwMakeContextCurrent(mWindowHandle);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        if (!status) {
            throw std::runtime_error("Failed to initialize glad!");
        }
    }

    void OpenGLContext::swapBuffers() {
        glfwSwapBuffers(mWindowHandle);
    }

}
