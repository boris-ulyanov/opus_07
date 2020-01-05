/**
 * @brief Buffer for accumulate commands
 */

#pragma once

#include <string>
#include <vector>

class Storage {
    std::vector<std::string> data;

    Storage() {
        // data.reserve(8);
    }

   public:
    Storage(const Storage& root) = delete;
    Storage& operator=(const Storage&) = delete;

    static Storage& Instance() {
        static Storage instance;
        return instance;
    }

    auto size() const {
        return data.size();
    }
    void add(const std::string& s) {
        data.push_back(s);
    }
    void clear() {
        data.clear();
    }
};
