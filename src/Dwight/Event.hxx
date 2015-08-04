//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-31
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_EVENT_HXX
#define HOWARD11_EVENT_HXX

#include <memory>

namespace Howard {

class Node;

enum EventType {
    ENone,
    EFoundation,
    EScriptEvent,
    EEnd
};

typedef int EventTypeExt;

class Event {

public:
    typedef std::shared_ptr<Event> shared_ptr_t;

    virtual ~Event() { }

    virtual EventType event_type() const { return EFoundation; }
    virtual EventTypeExt event_type_ext() const { return EventType::EFoundation; }

    Node *source() const { return this->m_source; }
    void set_source(Node *source) { this->m_source = source; }

    Node *root() const { return this->m_root; }
    void set_root(Node *root) { this->m_root = root; }

    void stop_propagation() { this->m_stopped = true; }
    bool stopped() const { return this->m_stopped; }

private:

    Node *m_source = nullptr;
    Node *m_root = nullptr;
    bool m_stopped = false;
};

}

#endif // HOWARD11_EVENT_HXX
