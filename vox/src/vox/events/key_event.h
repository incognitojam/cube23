#pragma once

#include "vox/events/event.h"

#include <sstream>

namespace Vox {
    class KeyEvent : public Event {
    public:
        [[nodiscard]] inline unsigned int getKeyCode() const { return mKeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        explicit KeyEvent(unsigned int keyCode)
            : mKeyCode(keyCode) {}

        unsigned int mKeyCode;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(unsigned int keyCode, unsigned int repeatCount) :
            KeyEvent(keyCode), mRepeatCount(repeatCount) {}

        [[nodiscard]] inline unsigned int getRepeatCount() const { return mRepeatCount; }

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        unsigned int mRepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(unsigned int keyCode) :
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
        explicit KeyTypedEvent(unsigned int keyCode) :
            KeyEvent(keyCode) {}

        [[nodiscard]] std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << mKeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}
