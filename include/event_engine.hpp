#pragma once

#include <cstdint>
#include <functional>

namespace EventEngine {

enum class EventType : std::uint8_t { WindowQuitEvent, SceneSwitchEvent };

class Event {};
class WindowQuitEvent : public Event {};
class SceneSwitchEvent : public Event {
  public:
    std::uint8_t to;
};

template <EventType>
struct HandlerSignature;

template <>
struct HandlerSignature<EventType::WindowQuitEvent> {
    using handler = std::function<void(const WindowQuitEvent& event)>;
};
template <>
struct HandlerSignature<EventType::SceneSwitchEvent> {
    using handler = std::function<void(const SceneSwitchEvent& event)>;
};

template <EventType E>
using EventHandler = typename HandlerSignature<E>::handler;

class EventEngine {
  private:
    template <EventType E>
    struct HandlerStorage {
        std::vector<EventHandler<E>> event_handlers;
    };
    template <EventType E>
    HandlerStorage<E>& get_storage() {
        static HandlerStorage<E> storage;
        return storage;
    };

  public:
    template <EventType E>
    void on(EventHandler<E> handler) {
        EventEngine::HandlerStorage<E>& storage = get_storage<E>();
        storage.event_handlers.emplace_back(std::move(handler));
    }
    template <EventType E, typename... Args>
    void emit(Args&&... args) {
        EventEngine::HandlerStorage<E>& storage = get_storage<E>();
        for (EventHandler<E>& handler : storage.event_handlers) {
            handler(std::forward<Args>(args)...);
        }
    }
};

}
