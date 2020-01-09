/**
 * @brief Output packed commands to term
 */

#pragma once

#include "emitter.h"
#include "storage.h"

#include <iostream>

class OutToTerm : Observer {
   public:
    OutToTerm() {
        Emitter::Instance().subscribe(Event::END_PACK, this);
    }

    void on_event([[maybe_unused]] Event e) override {
        const auto& stor = Storage::Instance();
        const auto size = stor.size();
        std::cout << "bulk: ";
        if (size > 0) {
            std::cout << stor[0];
            for (std::size_t i = 1; i < size; ++i) std::cout << ", " << stor[i];
        }
        std::cout << std::endl;
    }
};
