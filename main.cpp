
#include "out_to_file.h"
#include "out_to_term.h"
#include "string_processor.h"

#include <assert.h>
#include <iostream>

int main(int argc, const char* argv[]) {
    int default_pack_size = 3;
    if (argc >= 2) {
        default_pack_size = std::atoi(argv[1]);
        assert(default_pack_size > 0);
    }

    StringProcessor processor(default_pack_size);

    OutToTerm out_to_term;
    OutToFile out_to_file;

    for (std::string line; std::getline(std::cin, line);) {
        auto s = std::make_shared<std::string>(std::move(line));
        processor.add_line(s);
    }
    processor.eof();

    return 0;
}
