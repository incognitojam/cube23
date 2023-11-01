#pragma once

#include "vox/core.h"

#include <functional>
#include <string>

namespace Vox {
    enum class EventType {
        None = 0,
        WindowMoved, WindowResize, WindowFocus, WindowLostFocus, WindowClose,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased,
        MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled,
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4),
    };

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; } \
                               virtual EventType getEventType() const override { return getStaticType(); } \
                               virtual const char *getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

    class Event {
        friend class EventDispatcher;

    public:
        [[nodiscard]] virtual EventType getEventType() const = 0;
        [[nodiscard]] virtual const char *getName() const = 0;
        [[nodiscard]] virtual int getCategoryFlags() const = 0;

        [[nodiscard]] virtual std::string toString() const { return getName(); }

        [[nodiscard]] inline bool isInCategory(EventCategory category) const {
            return getCategoryFlags() & category;
        }

    protected:
        bool mHandled = false;
    };

    class EventDispatcher {
        template<typename T>
        using EventFn = std::function<bool(T &)>;

    public:
        explicit EventDispatcher(Event &event) : mEvent(event) {}

        template<typename T>
        bool dispatch(EventFn<T> func) {
            if (mEvent.getEventType() == T::getStatictype()) {
                mEvent.mHandled = func(*(T *) &mEvent);
                return true;
            }
            return false;
        }

    private:
        Event &mEvent;
    };

    inline std::ostream &operator<<(std::ostream &os, const Event &e) {
        return os << e.toString();
    }
};
