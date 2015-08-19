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

class HNode;

enum EventType {
    ENone,
    EFoundation,
    EInputEvent,
    EScriptEvent,
    EEnd
};

typedef int EventTypeExt;

class HEvent {

public:
    typedef std::shared_ptr<HEvent> shared_ptr_t;

    // ATTENTION: you should not make this
    //  (as a foundation class) directly instantiatable
    static inline shared_ptr_t createShared() { return std::make_shared<HEvent>(); }
    virtual ~HEvent() { }

    virtual EventType event_type() const { return EFoundation; }
    virtual EventTypeExt event_type_ext() const { return EventType::EFoundation; }

    HNode *source() const { return this->m_source; }
    void set_source(HNode *source) { this->m_source = source; }

    // indicates the node it starts propagation
    HNode *root() const { return this->m_root; }
    void set_root(HNode *root) { this->m_root = root; }

    void stop_propagation() { this->m_stopped = true; }
    bool stopped() const { return this->m_stopped; }

private:

    HNode *m_source = nullptr;
    HNode *m_root = nullptr;
    bool m_stopped = false;
};

}

#endif // HOWARD11_EVENT_HXX
