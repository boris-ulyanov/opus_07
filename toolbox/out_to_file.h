/**
 * @brief Output packed commands to file
 */

#pragma once

#include "emitter.h"
#include "storage.h"

#include <ctime>
#include <fstream>

class OutToFile : Observer {
    std::time_t first_cmd__time;

    void on_first_command() {
        first_cmd__time = std::time(nullptr);
    }

    void on_end_pack() {
        const auto filename = std::to_string(first_cmd__time) + std::string(".log");
        std::ofstream file;
        file.open(filename);

        const auto& stor = Storage::Instance();
        const auto size = stor.size();
        for (std::size_t i = 0; i < size; ++i) file << stor[i] << std::endl;
        file.close();
    }

   public:
    OutToFile() {
        first_cmd__time = -1;
        Emitter::Instance().subscribe(Event::FIRST_COMMAND, this);
        Emitter::Instance().subscribe(Event::END_PACK, this);
    }

    void on_event([[maybe_unused]] Event e) override {
        if (e == Event::FIRST_COMMAND)
            on_first_command();
        else
            on_end_pack();
    }
};
