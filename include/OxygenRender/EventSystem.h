#pragma once
#include <queue>
#include <variant>
#include <glm/vec2.hpp>
#include <unordered_map>
#include "OxygenRender/GraphicsTypes.h"
#include <GLFW/glfw3.h>

namespace OxyRender
{
    enum class KeyCode
    {
        Unknown = GLFW_KEY_UNKNOWN,
        Enter = GLFW_KEY_ENTER,
        Escape = GLFW_KEY_ESCAPE,
        LeftAlt = GLFW_KEY_LEFT_ALT,
        LeftShift = GLFW_KEY_LEFT_SHIFT,
        RightAlt = GLFW_KEY_RIGHT_ALT,
        RightShift = GLFW_KEY_RIGHT_SHIFT,
        Space = GLFW_KEY_SPACE,
        Tab = GLFW_KEY_TAB,
        Left = GLFW_KEY_LEFT,
        Right = GLFW_KEY_RIGHT,
        Up = GLFW_KEY_UP,
        Down = GLFW_KEY_DOWN,
        PageUp = GLFW_KEY_PAGE_UP,
        PageDown = GLFW_KEY_PAGE_DOWN,
        Home = GLFW_KEY_HOME,
        End = GLFW_KEY_END,
        Delete = GLFW_KEY_DELETE,
        Insert = GLFW_KEY_INSERT,
        F1 = GLFW_KEY_F1,
        F2 = GLFW_KEY_F2,
        F3 = GLFW_KEY_F3,
        F4 = GLFW_KEY_F4,
        F5 = GLFW_KEY_F5,
        F6 = GLFW_KEY_F6,
        F7 = GLFW_KEY_F7,
        F8 = GLFW_KEY_F8,
        F9 = GLFW_KEY_F9,
        F10 = GLFW_KEY_F10,
        A = GLFW_KEY_A,
        B = GLFW_KEY_B,
        C = GLFW_KEY_C,
        D = GLFW_KEY_D,
        E = GLFW_KEY_E,
        F = GLFW_KEY_F,
        G = GLFW_KEY_G,
        H = GLFW_KEY_H,
        I = GLFW_KEY_I,
        J = GLFW_KEY_J,
        K = GLFW_KEY_K,
        L = GLFW_KEY_L,
        M = GLFW_KEY_M,
        N = GLFW_KEY_N,
        O = GLFW_KEY_O,
        P = GLFW_KEY_P,
        Q = GLFW_KEY_Q,
        R = GLFW_KEY_R,
        S = GLFW_KEY_S,
        T = GLFW_KEY_T,
        U = GLFW_KEY_U,
        V = GLFW_KEY_V,
        W = GLFW_KEY_W,
        X = GLFW_KEY_X,
        Y = GLFW_KEY_Y,
        Z = GLFW_KEY_Z
    };
    static KeyCode glfwToKeyCode(int key)
    {
        return static_cast<KeyCode>(key);
    }
    static int keyCodetoGLFW(KeyCode k)
    {
        return static_cast<int>(k);
    }
    enum class MouseCode
    {
        ButtonLeft = GLFW_MOUSE_BUTTON_LEFT,
        ButtonRight = GLFW_MOUSE_BUTTON_RIGHT,
        ButtonMiddle = GLFW_MOUSE_BUTTON_MIDDLE
    };
    static MouseCode glfwToMouseCode(int button)
    {
        return static_cast<MouseCode>(button);
    }
    static int mouseCodeToGLFW(MouseCode button)
    {
        return static_cast<int>(button);
    }
    enum class EventType
    {
        None,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseScrolled,
        MouseMoved,
        WindowResized
    };

    struct KeyEvent
    {
        KeyCode key;
        int scancode;
        int mods;
    };

    struct MouseButtonEvent
    {
        int button;
        int mods;
    };
    struct MouseScrollEvent
    {
        float xoffset;
        float yoffset;
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
        std::variant<std::monostate, KeyEvent, MouseButtonEvent, MouseScrollEvent, MouseMoveEvent, WindowResizeEvent> data;
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

        static bool isKeyDown(KeyCode key);
        static bool isMouseButtonDown(MouseCode button);
    };
}
