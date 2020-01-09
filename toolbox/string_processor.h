/**
 * @brief Input string processor
 */

#pragma once

#include "emitter.h"
#include "storage.h"

class StringProcessor {
    const char SYM_START_PACK = '{';
    const char SYM_FINISH_PACK = '}';

    // не масштабируется, но что делать, если у каждого хэндлера свои параметры ???
    class CommandHandler* fixed_handler;
    class CommandHandler* dynamic_handler;

    class CommandHandler* current_handler;

   public:
    StringProcessor(std::size_t fixed_pack_size);
    ~StringProcessor();

    void set_handler_to_fixed();
    void set_handler_to_dynamic();

    void add(const std::string&);
    void finalize_pack();
};

class CommandHandler {
   public:
    virtual ~CommandHandler(){};
    virtual void start_pack(StringProcessor*) = 0;
    virtual void finish_pack(StringProcessor*) = 0;
    virtual void add_command(StringProcessor*, const std::string&) = 0;
};

class FixedHandler : public CommandHandler {
    std::size_t fixed_pack_size;

   public:
    FixedHandler(size_t _fix_pack_size) : fixed_pack_size(_fix_pack_size) {
    }

    ~FixedHandler() {
    }

    void start_pack(StringProcessor* sp) override {
        sp->finalize_pack();
        sp->set_handler_to_dynamic();
    }

    void finish_pack(StringProcessor*) override {
        // ignore
    }

    void add_command(StringProcessor* sp, const std::string& s) override {
        auto& stor = Storage::Instance();
        stor.add(s);

        auto size = stor.size();
        if (size == 1) Emitter::Instance().emit(Event::FIRST_COMMAND);
        if (size == fixed_pack_size) sp->finalize_pack();
    }
};

class DynamicHandler : public CommandHandler {
    int level;

   public:
    DynamicHandler() : level(0) {
    }

    ~DynamicHandler() {
    }

    void start_pack(StringProcessor*) override {
        ++level;
    }

    void finish_pack(StringProcessor* sp) override {
        if (level > 0) {
            --level;
            return;
        }
        sp->finalize_pack();
        sp->set_handler_to_fixed();
        return;
    }

    void add_command(StringProcessor*, const std::string& s) override {
        auto& stor = Storage::Instance();
        stor.add(s);

        auto size = stor.size();
        if (size == 1) Emitter::Instance().emit(Event::FIRST_COMMAND);
    }
};

StringProcessor::StringProcessor(std::size_t fixed_pack_size) {
    fixed_handler = new FixedHandler(fixed_pack_size);
    dynamic_handler = new DynamicHandler();
    current_handler = fixed_handler;
}

StringProcessor::~StringProcessor() {
    delete fixed_handler;
    delete dynamic_handler;
}

void StringProcessor::set_handler_to_fixed() {
    current_handler = fixed_handler;
}

void StringProcessor::set_handler_to_dynamic() {
    current_handler = dynamic_handler;
}

void StringProcessor::add(const std::string& s) {
    if (!s.empty()) {
        if (s[0] == SYM_START_PACK) return current_handler->start_pack(this);
        if (s[0] == SYM_FINISH_PACK) return current_handler->finish_pack(this);
    }
    current_handler->add_command(this, s);
}

void StringProcessor::finalize_pack() {
    auto& stor = Storage::Instance();
    if (stor.size() == 0) return;
    Emitter::Instance().emit(Event::END_PACK);
    stor.clear();
}
