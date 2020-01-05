/**
 * @brief Event emitter
 */

#pragma once

#include <cstdint>

enum class Event : uint8_t {
    NONE = 0,
    FIRST_COMMAND,
    END_PACK,
    COUNT,
};

class Emitter {
    Emitter() {
    }

   public:
    Emitter(const Emitter& root) = delete;
    Emitter& operator=(const Emitter&) = delete;

    static Emitter& Instance() {
        static Emitter instance;
        return instance;
    }

    void emit(const Event e) const {
    }

    void subscribe(const Event e, int todo) {
    }
};
