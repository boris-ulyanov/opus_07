/**
 * @brief Input string processor
 */

#pragma once

#include "emitter.h"
#include "storage.h"

class StringProcessor {
    class CommandHandler* current_handler;

   public:
    std::size_t fixed_pack_size;  // где хранить (по сути - это конфигурация одного из обработчиков) ???

    StringProcessor(std::size_t);
    ~StringProcessor();

    void set_handler(CommandHandler*);

    void add_line(const std::string&);
    void finalize_pack();
    void eof();
};

class CommandHandler {
   public:
    virtual ~CommandHandler() = default;
    virtual void start_pack(StringProcessor*) = 0;
    virtual void finish_pack(StringProcessor*) = 0;
    virtual void add_command(StringProcessor*, const std::string&) = 0;
    virtual void eof(StringProcessor*) = 0;
};

class FixedHandler : public CommandHandler {
   public:
    FixedHandler() = default;

    void start_pack(StringProcessor* sp) override;

    void finish_pack(StringProcessor*) override {
        // ignore
    }

    void add_command(StringProcessor* sp, const std::string& s) override {
        auto& stor = Storage::Instance();
        stor.add(s);

        auto size = stor.size();
        if (size == 1) Emitter::Instance().emit(Event::FIRST_COMMAND);
        if (size == sp->fixed_pack_size) sp->finalize_pack();
    }

    void eof(StringProcessor* sp) override {
        sp->finalize_pack();
    }
};

class DynamicHandler : public CommandHandler {
    int level;

   public:
    DynamicHandler() : level(0) {
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
        sp->set_handler(new FixedHandler());
    }

    void add_command(StringProcessor*, const std::string& s) override {
        auto& stor = Storage::Instance();
        stor.add(s);

        auto size = stor.size();
        if (size == 1) Emitter::Instance().emit(Event::FIRST_COMMAND);
    }

    void eof([[maybe_unused]] StringProcessor* sp) override {
        // ignore
    }
};

void FixedHandler::start_pack(StringProcessor* sp) {
    sp->finalize_pack();
    sp->set_handler(new DynamicHandler());
}

StringProcessor::StringProcessor(std::size_t _fixed_pack_size) {
    fixed_pack_size = _fixed_pack_size;
    current_handler = new FixedHandler();
}

StringProcessor::~StringProcessor() {
    delete current_handler;
}

void StringProcessor::set_handler(CommandHandler* new_handler) {
    delete current_handler;
    current_handler = new_handler;
}

void StringProcessor::add_line(const std::string& s) {
    const char SYM_START_PACK = '{';
    const char SYM_FINISH_PACK = '}';

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

void StringProcessor::eof() {
    current_handler->eof(this);
}
