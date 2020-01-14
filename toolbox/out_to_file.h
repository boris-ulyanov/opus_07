/**
 * @brief Output packed commands to file
 */

#pragma once

#include "emitter.h"

#include <ctime>
#include <fstream>

class OutToFile : Observer {
    std::time_t first_cmd_time;
    std::vector<shared_string> commands;

   public:
    OutToFile() {
        first_cmd_time = -1;
        Emitter::Instance().subscribe(Event::COMMAND, this);
        Emitter::Instance().subscribe(Event::END_PACK, this);
    }

    void on_event([[maybe_unused]] Event e) override {
        assert(e == Event::END_PACK);
        if (commands.empty()) return;

        const auto filename = std::string("bulk") + std::to_string(first_cmd_time) + std::string(".log");
        std::ofstream file;
        file.open(filename);

        for (const auto& s: commands)
            file << *s << std::endl;

        file.close();
        commands.clear();
    }

    void on_event([[maybe_unused]] Event e, const shared_string& s) override {
        assert(e == Event::COMMAND);

        if (commands.empty())
            first_cmd_time = std::time(nullptr);

        commands.push_back(s);
    }
};
