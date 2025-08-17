#include "OxygenRender/EventSystem.h"

namespace OxyRender
{
    std::queue<Event> EventSystem::m_events;
    std::unordered_map<int, bool> EventSystem::m_keyStates;
    std::unordered_map<int, bool> EventSystem::m_mouseButtonStates;

    void EventSystem::pushEvent(const Event &e)
    {
        if (e.type == EventType::KeyPressed)
        {
            auto data = std::get<KeyEvent>(e.data);
            m_keyStates[data.key] = true;
        }
        else if (e.type == EventType::KeyReleased)
        {
            auto data = std::get<KeyEvent>(e.data);
            m_keyStates[data.key] = false;
        }
        else if (e.type == EventType::MouseButtonPressed)
        {
            auto data = std::get<MouseButtonEvent>(e.data);
            m_mouseButtonStates[data.button] = true;
        }
        else if (e.type == EventType::MouseButtonReleased)
        {
            auto data = std::get<MouseButtonEvent>(e.data);
            m_mouseButtonStates[data.button] = false;
        }

        m_events.push(e);
    }

    bool EventSystem::pollEvent(Event &e)
    {
        if (m_events.empty())
            return false;

        e = m_events.front();
        m_events.pop();
        return true;
    }

    void EventSystem::clear()
    {
        while (!m_events.empty())
            m_events.pop();
    }

    bool EventSystem::isKeyDown(int key)
    {
        auto it = m_keyStates.find(key);
        return it != m_keyStates.end() && it->second;
    }

    bool EventSystem::isMouseButtonDown(int button)
    {
        auto it = m_mouseButtonStates.find(button);
        return it != m_mouseButtonStates.end() && it->second;
    }
}
