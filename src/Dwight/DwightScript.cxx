//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-17
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "Common.hxx"

#include "ScriptNode.hxx"
#include "ScriptEvent.hxx"
#include "../thirdpt/mozjs.hxx"

#include <memory>

namespace Howard {

using namespace JS;
using namespace xoundation::spd;

using casterSharedEvent = caster<HEvent::shared_ptr_t>;
using casterSharedScriptEvent = caster<ScriptEventBase::shared_ptr_t>;

// for ScriptNode, we call the 'onUpdate', 'onEvent' and 'onScriptEvent'
//  functions in its intrusive object.
//
// for EventListenerScript, we call the PersistentRooted callback functions.
//
void ScriptNodeBase::onUpdate() {
    RootedObject self(m_scriptctx, get_intrusive_object(this));
    RootedValue rval(m_scriptctx);
    JS_CallFunctionName(m_scriptctx, self, "onUpdate", JS::HandleValueArray::empty(), &rval);
}

void ScriptNodeBase::onEvent(HEvent::shared_ptr_t event) {
    RootedObject self(m_scriptctx, get_intrusive_object(this));
    RootedValue event_script(m_scriptctx, casterSharedEvent::tojs(m_scriptctx, event));
    RootedValue rval(m_scriptctx);
    JS_CallFunctionName(m_scriptctx, self, "onEvent", event_script, &rval);
}

void ScriptNodeBase::onScriptEvent(ScriptEventBase::shared_ptr_t event) {
    RootedObject self(m_scriptctx, get_intrusive_object(this));
    RootedValue event_script(m_scriptctx, casterSharedScriptEvent::tojs(m_scriptctx, event));
    RootedValue rval(m_scriptctx);
    JS_CallFunctionName(m_scriptctx, self, "onScriptEvent", event_script, &rval);
}

void EventListenerScript::onEvent(HEvent::shared_ptr_t event) {
    RootedObject self(m_context, scriptObject);
    RootedValue event_script(m_context, casterSharedEvent::tojs(m_context, event));
    RootedValue rval(m_context);
    JS_CallFunctionName(m_context, self, "onEvent", event_script, &rval);
}

void EventListenerScript::onScriptEvent(ScriptEventBase::shared_ptr_t event) {
    RootedObject self(m_context, scriptObject);
    RootedValue event_script(m_context, casterSharedScriptEvent::tojs(m_context, event));
    RootedValue rval(m_context);
    JS_CallFunctionName(m_context, self, "onScriptEvent", event_script, &rval);
}

}
