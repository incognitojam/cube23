#pragma once

#include <utility>

#include "vox/core.h"

namespace Vox {
    class Input {
    public:
        static bool isKeyPressed(unsigned int keycode) { return sInstance->isKeyPressedImpl(keycode); }

        inline static bool isMouseButtonPressed(unsigned int button) {
            return sInstance->isMouseButtonPressedImpl(button);
        }

        inline static std::pair<float, float> getMousePosition() { return sInstance->getMousePositionImpl(); }

        inline static float getMouseX() { return sInstance->getMouseXImpl(); }

        inline static float getMouseY() { return sInstance->getMouseYImpl(); }

    protected:
        bool isKeyPressedImpl(unsigned int keycode);

        bool isMouseButtonPressedImpl(unsigned int button);
        std::pair<float, float> getMousePositionImpl();

        float getMouseXImpl();
        float getMouseYImpl();

    private:
        static Input *sInstance;
    };
}
