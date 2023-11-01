#pragma once

#include "vox/events/event.h"

#include <sstream>

namespace Vox {
    class MouseEvent : public Event {
    public:
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class MouseMovedEvent : public MouseEvent {
    public:
        MouseMovedEvent(float x, float y)
            : mMouseX(x), mMouseY(y) {}

        [[nodiscard]] inline float getX() const { return mMouseX; }

        [[nodiscard]] inline float getY() const { return mMouseY; }

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)

    private:
        float mMouseX, mMouseY;
    };

    class MouseScrolledEvent : public MouseEvent {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : mXOffset(xOffset), mYOffset(yOffset) {}

        [[nodiscard]] inline float getXOffset() const { return mXOffset; }

        [[nodiscard]] inline float getYOffset() const { return mYOffset; }

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)

    private:
        float mXOffset, mYOffset;
    };

    class MouseButtonEvent : public Event {
    public:
        [[nodiscard]] inline unsigned int getMouseButton() const { return mButton; }

        EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)

    protected:
        explicit MouseButtonEvent(unsigned int button)
            : mButton(button) {}

        unsigned int mButton;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonPressedEvent(unsigned int button)
            : MouseButtonEvent(button) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << mButton;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonReleasedEvent(unsigned int button)
            : MouseButtonEvent(button) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << mButton;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}
