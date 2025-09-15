#pragma once
#include <queue>
#include <variant>
#include <unordered_map>
#include <functional>
#include "OxygenRender/GraphicsTypes.h"
#include <GLFW/glfw3.h>

namespace OxyRender
{

    // 键盘按键枚举
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

    // 键盘按键与GLFW键值转换
    static KeyCode glfwToKeyCode(int key)
    {
        return static_cast<KeyCode>(key);
    }
    static int keyCodetoGLFW(KeyCode k)
    {
        return static_cast<int>(k);
    }

    // 鼠标按键枚举
    enum class MouseCode
    {
        ButtonLeft = GLFW_MOUSE_BUTTON_LEFT,
        ButtonRight = GLFW_MOUSE_BUTTON_RIGHT,
        ButtonMiddle = GLFW_MOUSE_BUTTON_MIDDLE
    };
    // 鼠标按键与GLFW键值转换
    static MouseCode glfwToMouseCode(int button)
    {
        return static_cast<MouseCode>(button);
    }
    static int mouseCodeToGLFW(MouseCode button)
    {
        return static_cast<int>(button);
    }
    // 事件枚举
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
    // Key事件数据结构体
    struct KeyEvent
    {
        KeyCode key;
        int scancode;
        int mods;
    };

    // MouseButton事件数据结构体
    struct MouseButtonEvent
    {
        int button;
        int mods;
    };
    // MouseScroll事件数据结构体
    struct MouseScrollEvent
    {
        float xOffset;
        float yOffset;
    };
    // MouseMove事件数据结构体
    struct MouseMoveEvent
    {
        float positionX;
        float positionY;
    };

    // WindowResize事件数据结构体
    struct WindowResizeEvent
    {
        int width;
        int height;
    };
    // 事件数据结构体
    struct Event
    {
        EventType type = EventType::None;
        std::variant<std::monostate, KeyEvent, MouseButtonEvent, MouseScrollEvent, MouseMoveEvent, WindowResizeEvent> data;
    };
    using KeyEventCallback = std::function<void(const KeyEvent &)>;
    using MouseButtonEventCallback = std::function<void(const MouseButtonEvent &)>;
    using MouseMoveEventCallback = std::function<void(const MouseMoveEvent &)>;
    using MouseScrollEventCallback = std::function<void(const MouseScrollEvent &)>;
    using WindowResizeEventCallback = std::function<void(const WindowResizeEvent &)>;
    // 事件系统类，处理事件的注册、分发和状态查询
    class EventSystem
    {
    private:
        std::queue<Event> m_events;
        std::unordered_map<int, bool> m_keyStates;
        std::unordered_map<int, bool> m_mouseButtonStates;
        std::unordered_map<EventType, std::function<void(const Event &)>> m_eventCallbacks;

        bool m_firstMouse;
        float m_mouseLastX;
        float m_mouseLastY;

        EventSystem();
        ~EventSystem() = default;
        EventSystem(const EventSystem &) = delete;
        EventSystem &operator=(const EventSystem &) = delete;

    public:
        static EventSystem &getInstance();
        void registerCallback(EventType type, std::function<void(const Event &)> callback);
        void handleEvent();
        void pushEvent(const Event &e);
        bool pollEvent(Event &e);
        void clear();

        bool isKeyDown(KeyCode key);
        bool isMouseButtonDown(MouseCode button);

        struct MouseDelta
        {
            float dx;
            float dy;
        };
        MouseDelta handleMouseMoved(const MouseMoveEvent &mouse);
        void resetMouse();
    };
}
