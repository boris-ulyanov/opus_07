/**
 * @brief Event emitter
 */

#pragma once

#include <assert.h>
#include <cstdint>
#include <vector>

enum class Event : uint8_t {
    NONE = 0,
    FIRST_COMMAND,
    END_PACK,
    COUNT,
};

class Observer {
   public:
    virtual void on_event(Event e) = 0;
};

class Emitter {
    using one_event_observers = std::vector<Observer*>;
    std::vector<one_event_observers> all_observers;

    Emitter() {
        all_observers.resize(static_cast<std::size_t>(Event::COUNT));
    }

   public:
    Emitter(const Emitter& root) = delete;
    Emitter& operator=(const Emitter&) = delete;

    static Emitter& Instance() {
        static Emitter instance;
        return instance;
    }

    void emit(const Event e) const {
        assert(e < Event::COUNT);
        const auto& observers = all_observers[static_cast<std::size_t>(e)];
        for (const auto& o : observers) o->on_event(e);
    }

    void subscribe(Event e, Observer* obs) {
        assert(e < Event::COUNT);
        all_observers[static_cast<std::size_t>(e)].push_back(obs);
    }
};
