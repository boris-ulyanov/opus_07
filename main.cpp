
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

    for (std::string line; std::getline(std::cin, line);) {
        processor.add(line);
    }

    return 0;
}
