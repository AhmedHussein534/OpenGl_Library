#include <unordered_map>
#include <functional>

#include "events/Event.hpp"

class EventDispatcher
{
    public:
    static EventDispatcher& getInstance()
    {
        static EventDispatcher eventDispatcher;
        return eventDispatcher;
    }

    void publishEvent(const Event &e)
    {
        for (auto &it : subscriptionMap)
        {
            if (e.IsInCategory(static_cast<EventCategory>(it.first)))
            {
                it.second(e);
            }
        }
    }

    template<typename T, typename F>
    bool Dispatch(const Event &m_Event, const F& func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.Handled |= func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

    bool subscribeToEvents(uint32_t categories, std::function<void(const Event&)> callback)
    {
        subscriptionMap[categories] = callback;
    }


    private:
    EventDispatcher() = default;
    std::unordered_map<uint32_t, std::function<void(const Event&)>> subscriptionMap;
};