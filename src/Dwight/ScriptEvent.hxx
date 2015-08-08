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

#ifndef HOWARD11_SCRIPTEVENT_HXX
#define HOWARD11_SCRIPTEVENT_HXX

#include "Event.hxx"
#include "Node.hxx"

#include <memory>
#include "../thirdpt/mozjs.hxx"

namespace Howard {

class ScriptEventBase final : public Event {

public:
    typedef std::shared_ptr<ScriptEventBase> shared_ptr_t;

    ScriptEventBase(xoundation::spd::context_reference context) :
            scriptObject(context) { }

    EventType event_type() const override { return EventType::EScriptEvent; }
    EventTypeExt event_type_ext() const override { return this->m_type_ext; }

    JS::PersistentRootedValue scriptObject;

private:

    EventTypeExt m_type_ext = static_cast<EventTypeExt>(EventType::EEnd) + 1;
};

class EventListenerScript final : public EventListener {
public:
    EventListenerScript(Node *parent) :
            EventListenerScript(parent, DEFAULT_PRIORITY) { }

    EventListenerScript(Node *parent, int priority) :
            EventListener(parent, priority) { }

    virtual void on_event(Event::shared_ptr_t event) override {

    }

private:

    // JS::PersistentRootedFunction m_callback;
};

}

#endif // HOWARD11_SCRIPTEVENT_HXX
