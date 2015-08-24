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

    static ScriptEventBase::shared_ptr_t createShared(EventTypeExt type, xoundation::spd::context_reference context) {
        return std::make_shared<ScriptEventBase>(type, context); }

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

class EventListenerScript final : public EventListenerBase {
public:

    static std::shared_ptr<EventListenerScript> createShared(
            const std::string& name, xoundation::spd::context_reference context) {
        return std::make_shared<EventListenerScript>(name, context); }

    EventListenerScript(const std::string& name, xoundation::spd::context_reference context) :
            EventListenerScript(name, DEFAULT_PRIORITY, context) { }
    EventListenerScript(const std::string& name, int priority, xoundation::spd::context_reference context) :
            EventListenerBase(priority), scriptObject(context), m_name(name), m_context(context) { }

    const char *listenerName() const override {
        return m_name.c_str(); }

    void onEvent(HEvent::shared_ptr_t event) override;
    void onScriptEvent(ScriptEventBase::shared_ptr_t event) override;

    JS::PersistentRootedObject scriptObject;

private:

    std::string m_name;
    JSContext *m_context;
};

struct EventListenerScriptDataBase {
    EventListenerScriptDataBase(std::shared_ptr<EventListenerScript> listener)
            : m_listener(listener) { }
    std::shared_ptr<EventListenerScript> get_listener() {
        return m_listener.lock(); }
private:
    std::weak_ptr<EventListenerScript> m_listener;
};

}

#endif // HOWARD11_SCRIPTEVENT_HXX
