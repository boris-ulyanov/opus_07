/**
 * @brief Output packed commands to term
 */

#pragma once

#include "emitter.h"

#include <iostream>

class OutToTerm : Observer {
    std::vector<shared_string> commands;

   public:
    OutToTerm() {
        Emitter::Instance().subscribe(Event::COMMAND, this);
        Emitter::Instance().subscribe(Event::END_PACK, this);
    }

    void on_event(Event e) override {
        assert(e == Event::END_PACK);

        const auto size = commands.size();
        if (size == 0) return;

        std::cout << "bulk: " << *commands[0];
        for (std::size_t i = 1; i < size; ++i)
            std::cout << ", " << *commands[i];
        std::cout << std::endl;

        commands.clear();
    }

    void on_event(Event e, const shared_string& s) override {
        assert(e == Event::COMMAND);
        commands.push_back(s);
    }
};
