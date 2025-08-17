#pragma once
#include <queue>
#include <variant>
#include <glm/vec2.hpp>
#include <unordered_map>

namespace OxyRender
{
    enum class EventType
    {
        None,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        WindowResized
    };

    struct KeyEvent
    {
        int key;
        int scancode;
        int mods;
    };

    struct MouseButtonEvent
    {
        int button;
        int mods;
    };

    struct MouseMoveEvent
    {
        glm::vec2 position;
    };

    struct WindowResizeEvent
    {
        int width;
        int height;
    };

    struct Event
    {
        EventType type = EventType::None;
        std::variant<std::monostate, KeyEvent, MouseButtonEvent, MouseMoveEvent, WindowResizeEvent> data;
    };

    class EventSystem
    {
    private:
        static std::queue<Event> m_events;
        static std::unordered_map<int, bool> m_keyStates;
        static std::unordered_map<int, bool> m_mouseButtonStates;

    public:
        static void pushEvent(const Event &e);
        static bool pollEvent(Event &e);
        static void clear();

        static bool isKeyDown(int key);
        static bool isMouseButtonDown(int button);
    };
}
