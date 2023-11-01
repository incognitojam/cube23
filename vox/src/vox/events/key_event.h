#pragma once

#include "vox/events/event.h"

#include <sstream>

namespace Vox {
    class KeyEvent : public Event {
    public:
        [[nodiscard]] inline int getKeyCode() const { return mKeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        explicit KeyEvent(int keyCode)
            : mKeyCode(keyCode) {}

        int mKeyCode;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(int keyCode, int repeatCount) :
            KeyEvent(keyCode), mRepeatCount(repeatCount) {}

        [[nodiscard]] inline int getRepeatCount() const { return mRepeatCount; }

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int mRepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(int keyCode) :
            KeyEvent(keyCode) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << mKeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent {
    public:
        explicit KeyTypedEvent(int keyCode) :
            KeyEvent(keyCode) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << mKeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}
