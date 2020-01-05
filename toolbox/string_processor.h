/**
 * @brief Input string processor
 */

#pragma once

#include "emitter.h"
#include "storage.h"

class StringProcessor {
    const char SYM_START_PACK = '{';
    const char SYM_FINISH_PACK = '}';

    enum class Ctrl : char {
        NONE,
        START_PACK,
        FINISH_PACK,
    };

    std::size_t fix_pack_size;
    int level;

    Ctrl str_to_ctrl(const std::string& s) const {
        if (s.empty()) return Ctrl::NONE;
        if (s[0] == SYM_START_PACK) return Ctrl::START_PACK;
        if (s[0] == SYM_FINISH_PACK) return Ctrl::FINISH_PACK;
        return Ctrl::NONE;
    }

    void finalize_pack() {
        Emitter::Instance().emit(Event::END_PACK);
        Storage::Instance().clear();
    }

    void proc_fixed_pack(const std::string& s) {
        auto& stor = Storage::Instance();
        auto ctrl = str_to_ctrl(s);

        if (ctrl == Ctrl::START_PACK) {  // switch to dynamic pack
            if (stor.size() > 0) finalize_pack();
            level = 1;
            return;
        }

        if (ctrl == Ctrl::FINISH_PACK) {  // ignore
            return;
        }

        // not ctrl => command
        stor.add(s);

        auto size = stor.size();
        if (size == 1) Emitter::Instance().emit(Event::FIRST_COMMAND);
        if (size == fix_pack_size) finalize_pack();
    }

    void proc_dynamic_pack(const std::string& s) {
        auto& stor = Storage::Instance();
        auto ctrl = str_to_ctrl(s);

        if (ctrl == Ctrl::START_PACK) {
            ++level;
            return;
        }

        if (ctrl == Ctrl::FINISH_PACK) {
            --level;
            if (level == 0) finalize_pack();
            return;
        }

        // not ctrl => command
        stor.add(s);
        if (stor.size() == 1) Emitter::Instance().emit(Event::FIRST_COMMAND);
    }

   public:
    StringProcessor(std::size_t pack_size) : fix_pack_size(pack_size), level(0) {
    }

    void add(const std::string& s) {
        if (level > 0)
            proc_dynamic_pack(s);
        else
            proc_fixed_pack(s);
    }
};
