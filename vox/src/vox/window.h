#pragma once

#include <GLFW/glfw3.h>

#include <string>

namespace Vox {
    class Window {
    public:
        explicit Window(const std::string &title, int width, int height);
        ~Window();

        void onUpdate();

        [[nodiscard]] inline unsigned int getWidth() const { return mWidth; }

        [[nodiscard]] inline unsigned int getHeight() const { return mHeight; }

        // Window attributes
        void setVSync(bool);
        bool isVSync() const;

        static Window *create(const std::string &title = "Vox Engine", int width = 800, int height = 600);

    private:
        GLFWwindow *mWindow;

        std::string mTitle;
        int mWidth, mHeight;
        bool mVSync;
    };
}
