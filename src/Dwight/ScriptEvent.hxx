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

#include <string>
#include <memory>
#include "../thirdpt/mozjs.hxx"

namespace Howard {

class ScriptEventBase final : public HEvent {

public:
    typedef std::shared_ptr<ScriptEventBase> shared_ptr_t;

    ScriptEventBase(EventTypeExt type, xoundation::spd::context_reference context) :
            scriptObject(context), m_type_ext(type) { }

    EventType event_type() const override {
        return EventType::EScriptEvent; }
    EventTypeExt event_type_ext() const override {
        return this->m_type_ext; }

    JS::PersistentRootedValue scriptObject;

private:
    EventTypeExt m_type_ext = static_cast<EventTypeExt>(EventType::EEnd) + 1;
};

class EventListenerScriptBase final : public EventListenerBase {
public:
    EventListenerScriptBase(HNode *parent, const std::string& name, xoundation::spd::context_reference context) :
            EventListenerScriptBase(parent, name, DEFAULT_PRIORITY, context) { }
    EventListenerScriptBase(HNode *parent, const std::string& name, int priority, xoundation::spd::context_reference context) :
            EventListenerBase(parent, priority), scriptObject(context), m_name(name), m_context(context) { }

    const char *listenerName() const override {
        return m_name.c_str(); }

    void onEvent(HEvent::shared_ptr_t event) override;
    void onScriptEvent(ScriptEventBase::shared_ptr_t event) override;

    JS::PersistentRootedObject scriptObject;

private:

    std::string m_name;
    JSContext *m_context;
};

struct EventListenerScriptData {
    EventListenerScriptData(std::shared_ptr<EventListenerScriptBase> listener)
            : m_listener(listener) { }
    std::shared_ptr<EventListenerScriptBase> get_listener() {
        return m_listener.lock(); }
private:
    std::weak_ptr<EventListenerScriptBase> m_listener;
};

}

#endif // HOWARD11_SCRIPTEVENT_HXX
