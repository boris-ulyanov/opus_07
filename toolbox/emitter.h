/**
 * @brief Event emitter
 */

#pragma once

#include <assert.h>
#include <cstdint>
#include <memory>
#include <vector>

using shared_string = std::shared_ptr<std::string>;

enum class Event : uint8_t {
    NONE = 0,
    COMMAND,
    END_PACK,
    EVENT_COUNT,
};

class Observer {
   public:
    virtual void on_event(Event e) = 0;
    virtual void on_event(Event e, const shared_string& s) = 0;
};

class Emitter {
    using one_event_observers = std::vector<Observer*>;
    std::vector<one_event_observers> all_observers;

    Emitter() {
        all_observers.resize(static_cast<std::size_t>(Event::EVENT_COUNT));
    }

   public:
    Emitter(const Emitter& root) = delete;
    Emitter& operator=(const Emitter&) = delete;

    static Emitter& Instance() {
        static Emitter instance;
        return instance;
    }

    void emit(const Event e) const {
        assert(e < Event::EVENT_COUNT);
        const auto& observers = all_observers[static_cast<std::size_t>(e)];
        for (const auto& o : observers) o->on_event(e);
    }

    void emit(const Event e, const shared_string& s) const {
        assert(e < Event::EVENT_COUNT);
        const auto& observers = all_observers[static_cast<std::size_t>(e)];
        for (const auto& o : observers) o->on_event(e, s);
    }

    void subscribe(Event e, Observer* obs) {
        assert(e < Event::EVENT_COUNT);
        all_observers[static_cast<std::size_t>(e)].push_back(obs);
    }
};
