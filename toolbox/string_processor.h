/**
 * @brief Input string processor
 */

#pragma once

#include "emitter.h"

class StringProcessor;

class CommandHandler {
   public:
    virtual ~CommandHandler() = default;
    virtual void start_pack(StringProcessor*) = 0;
    virtual void finish_pack(StringProcessor*) = 0;
    virtual void add_command(StringProcessor*, const shared_string&) = 0;
    virtual void eof(StringProcessor*) = 0;
};

class StringProcessor {
    CommandHandler* current_handler;

   public:
    std::size_t fixed_pack_size;  // где хранить (по сути - это конфигурация одного из обработчиков) ???
    explicit StringProcessor(std::size_t);
    ~StringProcessor();
    void set_handler(CommandHandler*);
    void add_line(const shared_string&);
    void eof();
};

class FixedHandler : public CommandHandler {
    std::size_t ctr = 0;

   public:
    void start_pack(StringProcessor*) override;
    void finish_pack(StringProcessor*) override;
    void add_command(StringProcessor*, const shared_string&) override;
    void eof(StringProcessor*) override;
};

class DynamicHandler : public CommandHandler {
    int level = 0;

   public:
    void start_pack(StringProcessor*) override {
        ++level;
    }

    void finish_pack(StringProcessor* sp) override {
        if (level > 0) {
            --level;
            return;
        }
        Emitter::Instance().emit(Event::END_PACK);
        sp->set_handler(new FixedHandler());
    }

    void add_command(StringProcessor*, const shared_string& s) override {
        Emitter::Instance().emit(Event::COMMAND, s);
    }

    void eof(StringProcessor*) override {} // ignore
};

void FixedHandler::start_pack(StringProcessor* sp) {
    Emitter::Instance().emit(Event::END_PACK);
    sp->set_handler(new DynamicHandler());
}

void FixedHandler::finish_pack(StringProcessor*) {} // ignore

void FixedHandler::add_command(StringProcessor* sp, const shared_string& s) {
    Emitter::Instance().emit(Event::COMMAND, s);
    ++ctr;

    if (ctr < sp->fixed_pack_size) return;

    Emitter::Instance().emit(Event::END_PACK);
    ctr = 0;
}

void FixedHandler::eof(StringProcessor*) {
    Emitter::Instance().emit(Event::END_PACK);
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

void StringProcessor::add_line(const shared_string& s) {
    const char SYM_START_PACK = '{';
    const char SYM_FINISH_PACK = '}';

    if (!s->empty()) {
        if ((*s)[0] == SYM_START_PACK) return current_handler->start_pack(this);
        if ((*s)[0] == SYM_FINISH_PACK) return current_handler->finish_pack(this);
    }
    current_handler->add_command(this, s);
}

void StringProcessor::eof() {
    current_handler->eof(this);
}
