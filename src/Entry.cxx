//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-22
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Debug.hxx"

#include <stdio.h>
#include <stdlib.h>

#include "thirdpt/mozjs.hxx"

#include "Dwight/Node.hxx"
#include "Dwight/RootNode.hxx"
#include "Dwight/StannumSpriteNode.hxx"
#include "Dwight/ScriptNode.hxx"
#include "Dwight/ScriptEvent.hxx"
#include "Dwight/InputEvent.hxx"

#include "Verdandi/GLFoundation.hxx"
#include "Misc/AtTheVeryBeginning.hxx"
#include "Misc/Transform.hxx"

#include "FSM/FSM.hxx"
#include "FSM/FSMHelper.hxx"

#include "GLFWEvents.hxx"
#include "JSFunctions.hxx"

#include "Dwight/Observers.hxx"
#include "Dwight/ObserversDef.hxx"

#include "Dwight/Foundation.hxx"

#include "Hammer/Hammer.hxx"
#include "Hammer/HammerActorNode.hxx"
#include "Hammer/HammerPrimitiveBody.hxx"
#include "Hammer/HammerHeightfield.hxx"

#include "Dune/Dune.hxx"
#include "Guardian/Guardian.hxx"

#include <string>
#include <memory>
#include <vector>

using namespace Howard::FSMHelper;

namespace Howard {
FoundationInstance Foundation;
}

Howard::Stannum::StannumRenderer renderer;

Howard::Dune::DuneTerrain *terrain;

#define SPD_DEF(target) decltype(target), (target)

using namespace xoundation;

int main() {
    Howard::FSM::init();
    Howard::AtTheVeryBeginning::SettingGlobal::instance()->load("config.rc");
    {
        SpdRuntime *srt = new xoundation::SpdRuntime();
        JS::PersistentRootedObject *global = new JS::PersistentRootedObject(*srt, srt->create_global());
        Howard::Foundation.setJSRuntime(srt).setJSGlobal(global);
    }
    {
        using namespace Howard;
        using namespace xoundation::spd;
        SpdRuntime& rt = Foundation.JSRuntime();
        JS::PersistentRootedObject& global = Foundation.JSGlobal();
        JSAutoRequest req(rt);
        JSAutoCompartment at_comp(rt, global);

        JS_DefineFunction(rt, global, "print", js_print, 0,
            JSPROP_ENUMERATE | JSPROP_PERMANENT | JSFUN_STUB_GSOPS);

        JS_DefineFunction(rt, global, "collectgarbage", function_callback_wrapper
                <decltype(js_collectgarbage), js_collectgarbage>::callback, 0,
                JSPROP_ENUMERATE | JSPROP_PERMANENT | JSFUN_STUB_GSOPS);

        JS_DefineFunction(rt, global, "cast", js_cast, 2,
                JSPROP_ENUMERATE | JSPROP_PERMANENT | JSFUN_STUB_GSOPS);

        enumeration<HowardRTTIType>().define(rt, global, "HowardRTTIType")
                .enumerator<HowardRTTIType::TBase>("TBase")
                .enumerator<HowardRTTIType::TNode>("TNode")
                .enumerator<HowardRTTIType::THandle>("THandle")
                .enumerator<HowardRTTIType::TAsset>("TAsset")
                .enumerator<HowardRTTIType::TEventQueueGlobal>("TEventQueueGlobal")
                .enumerator<HowardRTTIType::TEventNotificationCenter>("TEventNotificationCenter");

        enumeration<HowardNodeType>().define(rt, global, "HowardNodeType")
                .enumerator<HowardNodeType::NFoundation>("NFoundation")
                .enumerator<HowardNodeType::NRootNode>("NRootNode")
                .enumerator<HowardNodeType::NScriptNode>("NScriptNode")
                .enumerator<HowardNodeType::NStannumSpriteNode>("NStannumSpriteNode");

        // enum EventType - Event.hxx
        enumeration<EventType>().define(rt, global, "EventType")
                .enumerator<EventType::ENone>("ENone")
                .enumerator<EventType::EFoundation>("EFoundation")
                .enumerator<EventType::EInputEvent>("EInputEvent")
                .enumerator<EventType::EScriptEvent>("EScriptEvent")
                .enumerator<EventType::EHammerTransformEvent>
                        ("EHammerTransformEvent")
                .enumerator<EventType::EEnd>("EEnd");

        // enum Observers - Observers.hxx
        //
        // well, problems would reveal, when you concentrate all of interfaces
        //  here (sometimes like what they did, doing, and will be doing for Beijing)
        //  - you used 'TBase', 'TNode' for HowardBase RTTI, 'NFoundation', 'NRootNode'
        //      for NodeType, so does Event, why 'QueueTypeXX' for EventQueue?
        enumeration<EventQueueType>().define(rt, global, "EventQueueType")
                .enumerator<EventQueueType::QueueTypeGlobalEventBase>("QueueTypeGlobalEventBase")
                .enumerator<EventQueueType::QueueTypeGlobalEventBaseMapped>("QueueTypeGlobalEventBaseMapped")
                .enumerator<EventQueueType::QueueTypeUpdate>("QueueTypeUpdate")
                .enumerator<EventQueueType::QueueTypePaint>("QueueTypePaint")
                .enumerator<EventQueueType::QueueTypeKeyboard>("QueueTypeKeyboard")
                .enumerator<EventQueueType::QueueTypeMouseButton>("QueueTypeMouseButton")
                .enumerator<EventQueueType::QueueTypeMouseMove>("QueueTypeMouseMove")
                .enumerator<EventQueueType::QueueTypeEnd>("QueueTypeEnd");

        class_info<HowardBase>::inst_wrapper::set(new spd::class_info<HowardBase>(rt, "HowardBase"));
        klass<HowardBase>().define<>(global)
                .method<SPD_DEF(&HowardBase::WhatAmI)>("WhatAmI")
                .method<decltype(&HowardBase::class_name), &HowardBase::class_name>("class_name");

        class_info<Asset>::inst_wrapper::set(new spd::class_info<Asset>(rt, "Asset"));
        // stackoverflow.com/questions/13554496/partial-template-function-specification-in-c-works-but-why
        klass<Asset>().inherits<HowardBase>(global, spd::argpack<const std::string&>())
                .method<SPD_DEF(&Asset::asset_type)>("asset_type")
                .method<SPD_DEF(&Asset::name)>("name");

        class_info<AssetManager>::inst_wrapper::set(new spd::class_info<AssetManager>(rt, "AssetManager"));
        klass<AssetManager>().define<>(global)
                .method<Asset *(AssetManager::*)(const std::string&), &AssetManager::named>("named");

        class_info<Verdandi::TextureImage>::inst_wrapper::set(
                new spd::class_info<Verdandi::TextureImage>(rt, "TextureImage"));
        klass<Verdandi::TextureImage>().inherits<Asset>(global, spd::argpack<const std::string&>());

        class_info<Verdandi::Texture>::inst_wrapper::set(
                new spd::class_info<Verdandi::Texture>(rt, "Texture"));
        klass<Verdandi::Texture>().inherits<Asset>(global, spd::argpack<const std::string&, Verdandi::TextureImage *>())
                .static_func<SPD_DEF(Verdandi::Texture::createWithEntireImage)>("createWithEntireImage")
                .static_func<SPD_DEF(Verdandi::Texture::createWithPartialImage)>("createWithPartialImage");

        class_info<EventListenerBase>::inst_wrapper::set(new
                class_info<EventListenerBase>(rt, "EventListenerBase"));
        klass<EventListenerBase>().inherits<HowardBase>(global, argpack<HNode *>())
                .accessor<int, &EventListenerBase::priority, &EventListenerBase::setPriority>("priority")
                .accessor<const char *, &EventListenerBase::listenerName>("listenerName")
                .accessor<HNode *, &EventListenerBase::listenerParent>("listenerParent");

        // class EventListenerScript : EventListenerBase - ScriptEvent.hxx
        class_info<EventListenerScript>::inst_wrapper::set(new
                class_info<EventListenerScript>(rt, "EventListenerScript"));
        klass<EventListenerScript>().inherits<EventListenerBase>(global,
                argpack<const std::string&, context_reference>())
                .static_func<SPD_DEF(EventListenerScript::createShared)>("createShared")
                .property<JS::PersistentRootedObject, &EventListenerScript::scriptObject>("data");

        // struct EventListenerScriptData - ScriptEvent.hxx
        class_info<EventListenerScriptDataBase>::inst_wrapper::set(new
                class_info<EventListenerScriptDataBase>(rt, "EventListenerScriptDataBase"));
        klass<EventListenerScriptDataBase>().define(global, argpack<std::shared_ptr<EventListenerScript>>())
                .reproto("reproto")
                .attach("attachNew", argpack<std::shared_ptr<EventListenerScript>>())
                .accessor<std::shared_ptr<EventListenerScript>, &EventListenerScriptDataBase::get_listener>("listener");

        // class Node - Node.hxx
        class_info<HNode>::inst_wrapper::set(new spd::class_info<HNode>(rt, "Node"));
        klass<HNode>().inherits<EventListenerBase, spd::UseCXXLifetime>(global, argpack<RootNode *>())
                .static_func<SPD_DEF(HNode::create)>("create")
                .property<int, &HNode::RTTIID>("RTTIID")
                .method<SPD_DEF(&HNode::node_typeid)>("node_typeid")
                .method<SPD_DEF(&HNode::node_type)>("node_type")
                .method<SPD_DEF(&HNode::parent)>("parent")
                .method<SPD_DEF(&HNode::has_parent)>("has_parent")
                .method<SPD_DEF(&HNode::has_child)>("has_child")
                .method<SPD_DEF(&HNode::add_child)>("add_child")
                .method<SPD_DEF(&HNode::detach_child)>("detach_child")
                .method<SPD_DEF(&HNode::attach_to)>("attach_to")
                .method<SPD_DEF(&HNode::detach_from_parent)>("detach_from_parent")
                .method<SPD_DEF(&HNode::root)>("root")
                .accessor<size_t, &HNode::get_length>("length")
                .method<SPD_DEF(&HNode::child)>("child")
                .method<SPD_DEF(&HNode::invoke_event)>("invoke_event")
                .method<std::shared_ptr<EventListenerBase> (HNode:: *)
                        (EventType, std::shared_ptr<EventListenerBase>),
                        &HNode::addListener>("addListener");

        class_info<RootNode>::inst_wrapper::set(new spd::class_info<RootNode>(rt, "RootNode"));
        klass<RootNode>().inherits<HNode>(global, spd::argpack<>());

        // class HEvent - Event.hxx
        class_info<HEvent>::inst_wrapper::set(new spd::class_info<HEvent>(rt, "HEvent"));
        klass<HEvent>().define(global)
                .static_func<SPD_DEF(HEvent::createShared)>("createShared")
                .method<SPD_DEF(&HEvent::event_type)>("event_type")
                .method<SPD_DEF(&HEvent::event_type_ext)>("event_type_ext")
                .accessor<HNode *, &HEvent::source>("source")
                .method<SPD_DEF(&HEvent::root)>("root")
                .method<SPD_DEF(&HEvent::stop_propagation)>("stop_propagation")
                .method<SPD_DEF(&HEvent::stopped)>("stopped");

        // class ScriptNode : Node - ScriptNode.hxx
        class_info<ScriptNodeBase>::inst_wrapper::set(new class_info<ScriptNodeBase>(rt, "ScriptNodeBase"));
        klass<ScriptNodeBase>().inherits<HNode>(global, argpack<RootNode *, context_reference>())
                .static_func<SPD_DEF(ScriptNodeBase::create)>("create")
                .attach<UseCXXLifetime>("attachNew", argpack<RootNode *, context_reference>())
                .reproto("reproto");

        class_info<QueueGlobalEventBase>::inst_wrapper::set(new
                class_info<QueueGlobalEventBase>(rt, "QueueGlobalEventBase"));
        klass<QueueGlobalEventBase>().inherits<HowardBase>(global, argpack<>())
                .method<SPD_DEF(&QueueGlobalEventBase::queue_type)>("queue_type")
                .method<SPD_DEF(&QueueGlobalEventBase::queue_name)>("queue_name")
                .method<SPD_DEF(&QueueGlobalEventBase::inside)>("inside")
                .method<SPD_DEF(&QueueGlobalEventBase::add)>("add")
                .method<SPD_DEF(&QueueGlobalEventBase::remove)>("remove");

        class_info<QueueGlobalPaint>::inst_wrapper::set(new class_info<QueueGlobalPaint>(rt, "QueueGlobalPaint"));
        klass<QueueGlobalPaint>().inherits<QueueGlobalEventBase>(global, argpack<>());
        class_info<QueueGlobalUpdate>::inst_wrapper::set(new class_info<QueueGlobalUpdate>(rt, "QueueGlobalUpdate"));
        klass<QueueGlobalUpdate>().inherits<QueueGlobalEventBase>(global, argpack<>());

        // attempt of the relatively 'new' accessor interface of mozjs
        // class EventQueueManager - Observers.hxx
        class_info<EventQueueManager>::inst_wrapper::set(new spd::class_info<EventQueueManager>(rt, "EventQueueManager"));
        klass<EventQueueManager>().define<>(global)
                .accessor<QueueGlobalUpdate *, &EventQueueManager::queueUpdate>("queueUpdate")
                .accessor<QueueGlobalPaint *, &EventQueueManager::queuePaint>("queuePaint")
                .accessor<QueueGlobalEventBase *, &EventQueueManager::queueKeyboard>("queueKeyboard")
                .accessor<QueueGlobalEventBase *, &EventQueueManager::queueMouseButton>("queueMouseButton")
                .accessor<QueueGlobalEventBase *, &EventQueueManager::queueMouseMove>("queueMouseMove")
                .method<QueueGlobalEventBase *(EventQueueManager:: *)(unsigned int),
                    &EventQueueManager::queue> ("queue");

        class_info<FoundationInstance>::inst_wrapper::set(new spd::class_info<FoundationInstance>(rt, "FoundationInstance"));
        klass<FoundationInstance>().define<>(global)
                .accessor<RootNode&, &FoundationInstance::rootNode>("rootNode")
                .accessor<EventQueueManager&, &FoundationInstance::eventQueues>("eventQueues")
                .accessor<AssetManager&, &FoundationInstance::assetManager>("assetManager")
                .accessor<Hammer::HammerFoundation&, &FoundationInstance::hammerFoundation>("hammerFoundation")
                .accessor<Hammer::HammerScene&, &FoundationInstance::mainPhysScene>("mainPhysScene");
        {
            JS::RootedObject foundation_proto(rt, class_info<FoundationInstance>::instance()->jsc_proto);
            JS::RootedValue foundation(rt, spd::caster<FoundationInstance&>::tojs(rt, Foundation));
            JS::RootedObject foundation_obj(rt, foundation.toObjectOrNull());
            JS_DefineProperty(rt, global, "Foundation", foundation_obj,
                    JSPROP_PERMANENT | JSPROP_READONLY | JSPROP_ENUMERATE, nullptr, nullptr);
        }

        class_info<StannumSpriteNode>::inst_wrapper::set(
                new spd::class_info<StannumSpriteNode>(rt, "StannumSpriteNode"));
        klass<StannumSpriteNode>().inherits<HNode, spd::UseCXXLifetime>(
                        global, argpack<RootNode *, Verdandi::Texture *>())
                .static_func<SPD_DEF(StannumSpriteNode::create)>("create")
                .accessor<HAnyCoord, &StannumSpriteNode::position>("position")
                .method<SPD_DEF(&StannumSpriteNode::set_position)>("set_position");

        {
            using namespace Hammer;

            // using HAnyCoord = glm::vec3 - Common.hxx
            class_info<HAnyCoord>::inst_wrapper::set(new
                    spd::class_info<HAnyCoord>(rt, "HAnyCoord"));
            klass<HAnyCoord>().define<float, float, float>(global)
                    .property<float, &HAnyCoord::x>("x")
                    .property<float, &HAnyCoord::y>("y")
                    .property<float, &HAnyCoord::z>("z");

            // using HQuaternion = glm::quat - Common.hxx
            class_info<HQuaternion>::inst_wrapper::set(new
                    spd::class_info<HQuaternion>(rt, "HQuaternion"));
            klass<HQuaternion>().define<float, float, float, float>(global)
                    .static_func<SPD_DEF(Transform::createEuler)>("createEuler")
                    .static_func<HQuaternion (const HQuaternion&), glm::normalize>("normalize")
                    .static_func<float (const HQuaternion&), glm::length>("length")
                    .property<float, &HQuaternion::x>("x")
                    .property<float, &HQuaternion::y>("y")
                    .property<float, &HQuaternion::z>("z")
                    .property<float, &HQuaternion::w>("w");

            // class HammerFoundation - Hammer/Hammer.hxx
            class_info<HammerFoundation>::inst_wrapper::set(new
                    class_info<HammerFoundation>(rt, "HammerFoundation"));
            klass<HammerFoundation>().define(global)
                    .method<SPD_DEF(&HammerFoundation::initialize)>("initialize")
                    .accessor<Material *, &HammerFoundation::defaultMaterial>("defaultMaterial");

            class_info<HammerScene>::inst_wrapper::set(new
                    class_info<HammerScene>(rt, "HammerScene"));
            klass<HammerScene>().define(global)
                    .method<SPD_DEF(&HammerScene::initialize)>("initialize");

            // struct HammerTransform - Hammer/HammerTransform.hxx
            class_info<Transform>::inst_wrapper::set(new
                    spd::class_info<Transform>(rt, "Transform"));
            klass<Transform>().define(global)
                    .static_func<SPD_DEF(Transform::create)>("create")
                    .static_func<SPD_DEF(Transform::createDefault)>("createDefault")
                    .static_func<SPD_DEF(Transform::createPositioned)>("createPositioned")
                    .static_func<SPD_DEF(Transform::createRotated)>("createRotated")
                    .property<HAnyCoord, &Transform::position>("position")
                    .property<HQuaternion, &Transform::rotation>("rotation");

            // enum PrimitiveType - Hammer/HammerPrimitiveBody.hxx
            enumeration<PrimitiveType>().define(rt, global, "HammerPrimitiveType")
                    .enumerator<PrimitiveType::PNone>("PNone")
                    .enumerator<PrimitiveType::PSphere>("PSphere")
                    .enumerator<PrimitiveType::PBox>("PBox")
                    .enumerator<PrimitiveType::PCapsule>("PCapsule")
                    .enumerator<PrimitiveType::PEnd>("PEnd");

            // class Material - Hammer/HammerPrimitiveBody.hxx
            class_info<Material>::inst_wrapper::set(new
                    class_info<Material>(rt, "HammerMaterial"));
            klass<Material>().define(global, argpack<HammerFoundation&, float, float, float>());

            // class PrimitiveBody - Hammer/HammerPrimitiveBody.hxx
            class_info<PrimitiveBody>::inst_wrapper::set(new
                    class_info<PrimitiveBody>(rt, "HammerPrimitiveBody"));
            klass<PrimitiveBody>().define(global, argpack<Material *>())
                    .method<SPD_DEF(&PrimitiveBody::addSphere)>("addSphere")
                    .method<SPD_DEF(&PrimitiveBody::addBox)>("addBox")
                    .method<SPD_DEF(&PrimitiveBody::addCapsule)>("addCapsule");

            // class PrimitiveHelper - Hammer/HammerPrimitiveBody.hxx
            class_info<PrimitiveHelper>::inst_wrapper::set(new
                    spd::class_info<PrimitiveHelper>(rt, "HammerPrimitiveHelper"));
            klass<PrimitiveHelper>().define(global)
                    .static_func<SPD_DEF(PrimitiveHelper::attachPrimitivesToActor)>("attachPrimitivesToActor")
                    .static_func<SPD_DEF(PrimitiveHelper::attachHeightfieldToActor)>("attachHeightfieldToActor");

            // class HammerActorNodeBase : HNode - Hammer/HammerActorNode.hxx
            class_info<HammerActorNodeBase>::inst_wrapper::set(new
                    class_info<HammerActorNodeBase>(rt, "HammerActorNodeBase"));
            klass<HammerActorNodeBase>().inherits<HNode, UseCXXLifetime>(
                    global, argpack<RootNode *, const Transform&>())
                    .method<SPD_DEF(&HammerActorNodeBase::addToScene)>("addToScene")
                    .accessor<Transform, &HammerActorNodeBase::transform>("transform");

            // class HammerActorNodeStatic : HammerActorNodeBase - Hammer/HammerActorNode.hxx
            class_info<HammerActorNodeStatic>::inst_wrapper::set(new
                class_info<HammerActorNodeStatic>(rt, "HammerActorNodeStatic"));
            klass<HammerActorNodeStatic>().inherits<HammerActorNodeBase, UseCXXLifetime>(
                    global, argpack<RootNode *, const Transform&>());

            // class HammerActorNode : HNode - Hammer/HammerActorNode.hxx
            class_info<HammerActorNode>::inst_wrapper::set(new
                    spd::class_info<HammerActorNode>(rt, "HammerActorNode"));
            klass<HammerActorNode>().inherits<HammerActorNodeBase, spd::UseCXXLifetime>(
                    global, argpack<RootNode *, const Transform&>())
                    .method<SPD_DEF(&HammerActorNode::addForce)>("addForce")
                    .method<SPD_DEF(&HammerActorNode::addImpulse)>("addImpulse")
                    .method<SPD_DEF(&HammerActorNode::addAcceleration)>("addAcceleration")
                    .method<SPD_DEF(&HammerActorNode::setVelocity)>("setVelocity")
                    .accessor<float, &HammerActorNode::mass>("mass")
                    .accessor<float, &HammerActorNode::invMass>("invMass");

            // class HammerTransformEvent : HEvent - Hammer/HammerActorNode.hxx
            class_info<HammerTransformEvent>::inst_wrapper::set(new
                    class_info<HammerTransformEvent>(rt, "HammerTransformEvent"));
            klass<HammerTransformEvent>().inherits<HEvent>(global,
                            argpack<HammerActorNode *, const Transform&>())
                    .property<Transform, &HammerTransformEvent::transform>("transform");

            class_info<HammerHeightfield>::inst_wrapper::set(new
                    class_info<HammerHeightfield>(rt, "HammerHeightfield"));
            klass<HammerHeightfield>().define(global)
                    .method<SPD_DEF(&HammerHeightfield::setDataImage)>("setDataImage")
                    .method<SPD_DEF(&HammerHeightfield::generateHeightfield)>("generateHeightfield");
        }

        // class ScriptEventBase : Event - ScriptEvent.hxx
        class_info<ScriptEventBase>::inst_wrapper::set(new spd::class_info<ScriptEventBase>(rt, "ScriptEventBase"));
        klass<ScriptEventBase>().inherits<HEvent>(global, argpack<EventTypeExt, context_reference>())
                .static_func<SPD_DEF(ScriptEventBase::createShared)>("createShared")
                .property<JS::PersistentRootedValue, &ScriptEventBase::scriptObject>("data");

        // class InputEvent : Event - InputEvent.hxx
        class_info<InputEvent>::inst_wrapper::set(new class_info<InputEvent>(rt, "InputEvent"));
        klass<InputEvent>().inherits<HEvent>(global, argpack<const Facer::InputEvent&>())
                .accessor<Facer::InputEvent&, &InputEvent::getData>("data");

        // enum Facer::InputEvent::EventType - FacerCore/FacerEvent.hxx
        enumeration<Facer::InputEvent::EventType>().define(rt, global, "FacerEventType")
                .enumerator<Facer::InputEvent::NoType>("NoType")
                .enumerator<Facer::InputEvent::MouseMove>("MouseMove")
                .enumerator<Facer::InputEvent::MousePress>("MousePress")
                .enumerator<Facer::InputEvent::MouseRelease>("MouseRelease")
                .enumerator<Facer::InputEvent::MouseWheel>("MouseWheel")
                .enumerator<Facer::InputEvent::KeyPress>("KeyPress")
                .enumerator<Facer::InputEvent::KeyRelease>("KeyRelease")
                .enumerator<Facer::InputEvent::InputText>("InputText");

        // enum Facer::InputEvent::MouseButton - FacerCore/FacerEvent.hxx
        enumeration<Facer::InputEvent::MouseButton>().define(rt, global, "FacerMouseButton")
                .enumerator<Facer::InputEvent::NOBUTTON>("NOBUTTON")
                .enumerator<Facer::InputEvent::LEFT>("LEFT")
                .enumerator<Facer::InputEvent::RIGHT>("RIGHT")
                .enumerator<Facer::InputEvent::MIDDLE>("MIDDLE");

        // enum Facer::InputEvent::ButtonState - FacerCore/FacerEvent.hxx
        enumeration<Facer::InputEvent::ButtonState>().define(rt, global, "FacerButtonState")
                .enumerator<Facer::InputEvent::RELEASED>("RELEASED")
                .enumerator<Facer::InputEvent::PRESSED>("PRESSED");

        // struct Facer::InputEvent::MouseButtonState - FacerCore/FacerEvent.hxx
        class_info<Facer::InputEvent::MouseButtonState>::inst_wrapper::set(new
                class_info<Facer::InputEvent::MouseButtonState>(rt, "FacerEventMouseButtonState"));
        klass<Facer::InputEvent::MouseButtonState>().define<>(global)
                .property<Facer::InputEvent::ButtonState, &Facer::InputEvent::MouseButtonState::left>("left")
                .property<Facer::InputEvent::ButtonState, &Facer::InputEvent::MouseButtonState::right>("right")
                .property<Facer::InputEvent::ButtonState, &Facer::InputEvent::MouseButtonState::middle>("middle");

        // struct Facer::InputEvent::ModifierState - FacerCore/FacerEvent.hxx
        class_info<Facer::InputEvent::ModifierState>::inst_wrapper::set(new
                class_info<Facer::InputEvent::ModifierState>(rt, "FacerEventModifierState"));
        klass<Facer::InputEvent::ModifierState>().define<>(global)
                .property<bool, &Facer::InputEvent::ModifierState::alt>("alt")
                .property<bool, &Facer::InputEvent::ModifierState::shift>("shift")
                .property<bool, &Facer::InputEvent::ModifierState::meta>("meta")
                .property<bool, &Facer::InputEvent::ModifierState::control>("control");

        // class Facer::InputEvent - FacerCore/FacerEvent.hxx
        class_info<Facer::InputEvent>::inst_wrapper::set(new class_info<Facer::InputEvent>(rt, "FacerEvent"));
        klass<Facer::InputEvent>().define<>(global)
                // why this function placed here?
                .static_func<SPD_DEF(Facer::Port::createEvent)>("createEvent")
                .property<Facer::InputEvent::EventType, &Facer::InputEvent::type>("type")
                .property<Facer::InputEvent::MouseButton, &Facer::InputEvent::primaryButton>("primaryButton")
                .property<Facer::InputEvent::MouseButtonState, &Facer::InputEvent::buttonState>("buttonState")
                .property<Facer::InputEvent::ModifierState, &Facer::InputEvent::modifierState>("modifierState")
                .property<std::string, &Facer::InputEvent::input>("input")
                .property<long long, &Facer::InputEvent::timestamp>("timestamp")
                .method<SPD_DEF(&Facer::InputEvent::isMouseCursorEvent)>("isMouseCursorEvent")
                .method<SPD_DEF(&Facer::InputEvent::setCursorPosition<unsigned short>)>("setCursorPosition")
                .method<SPD_DEF(&Facer::InputEvent::setWheelDelta<float>)>("setWheelDelta")
                .accessor<unsigned short,
                    &Facer::InputEvent::getCursorPositionX<unsigned short>,
                    &Facer::InputEvent::setCursorPositionX<unsigned short>>("cursorPositionX")
                .accessor<unsigned short,
                    &Facer::InputEvent::getCursorPositionY<unsigned short>,
                    &Facer::InputEvent::setCursorPositionY<unsigned short>>("cursorPositionY")
                .accessor<float, &Facer::InputEvent::getWheelDeltaX<float>,
                        &Facer::InputEvent::setWheelDeltaX<float>>("wheelDeltaX")
                .accessor<float, &Facer::InputEvent::getWheelDeltaY<float>,
                        &Facer::InputEvent::setWheelDeltaY<float>>("wheelDeltaY");

        class_info<HCoord>::inst_wrapper::set(new spd::class_info<HCoord>(rt, "HCoord"));
        klass<HCoord>().define<int, int, int>(global)
                .property<int, &HCoord::x>("x")
                .property<int, &HCoord::y>("y")
                .property<int, &HCoord::z>("z");

        class_info<HPoint>::inst_wrapper::set(new spd::class_info<HPoint>(rt, "HPoint"));
        klass<HPoint>().define(global)
                .property<short, &HPoint::x>("x")
                .property<short, &HPoint::y>("y");

        class_info<HPixel>::inst_wrapper::set(new spd::class_info<HPixel>(rt, "HPixel"));
        klass<HPixel>().define(global, argpack<unsigned short, unsigned short>())
                .property<unsigned short, &HPixel::x>("x")
                .property<unsigned short, &HPixel::y>("y");
    }

    Howard::Foundation.setRootNode(new Howard::RootNode());
    Howard::Foundation.setAssetManager(new Howard::AssetManager());
    Howard::Foundation.setEventQueues(new Howard::EventQueueManager());

    Howard::Hammer::HammerFoundation *hammer_foundation =
            new Howard::Hammer::HammerFoundation();
    hammer_foundation->initialize();
    Howard::Foundation.setHammerFoundation(hammer_foundation);
    Howard::Hammer::HammerScene *hammer_scene = new Howard::Hammer::HammerScene();
    hammer_scene->initialize(&Howard::Foundation.hammerFoundation(),
            Howard::Hammer::HammerSceneCreateArgs { });
    Howard::Foundation.setMainPhysScene(hammer_scene);
//    hammer_scene->createGroundPlane();

    Howard::Foundation.eventQueues().initialize();

    Howard::HNode r(&Howard::Foundation.rootNode());
    r.attach_to(&Howard::Foundation.rootNode());

    assert(glfwInit());
    glfwSetErrorCallback(error_callback);

    using namespace Howard::AtTheVeryBeginning;

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Setting<SRendering>::instance()->glv_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Setting<SRendering>::instance()->glv_minor);

    GLFWwindow *window = glfwCreateWindow
            (Howard::AtTheVeryBeginning::Setting<Howard::AtTheVeryBeginning::WindowSetting>::instance()->width,
             Howard::AtTheVeryBeginning::Setting<Howard::AtTheVeryBeginning::WindowSetting>::instance()->height,
             Howard::AtTheVeryBeginning::Setting<Howard::AtTheVeryBeginning::WindowSetting>::instance()->title.c_str(),
                                           nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(-1);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, glfw_cursorcb);
    glfwSetFramebufferSizeCallback(window, glfw_fbsizecb);
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, glfw_mousecb);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Howard::Verdandi::gl_init();
    renderer.init();

    Howard::Foundation.setGuardianFoundation(new
            Howard::Guardian::GuardianFoundation());

    Howard::Verdandi::TextureImage *textureimage = new Howard::Verdandi::TextureImage("node");
    Howard::Verdandi::TextureImage *textureimage_dot = new Howard::Verdandi::TextureImage("dot");
    Howard::Verdandi::TextureImage *textureimage_TestUnit = new Howard::Verdandi::TextureImage("TestUnit");
    std::shared_ptr<Howard::Verdandi::TextureImage> textureimage_heightmap(
            new Howard::Verdandi::TextureImage("duneheightmap"));
    std::shared_ptr<Howard::Verdandi::TextureImage> textureimage_dunepretex(
            new Howard::Verdandi::TextureImage("dunepretex"));
    {
        using namespace Howard;
        using namespace Howard::Verdandi;

        auto t = Image::createFromFile("load", "assets/node.png", IRGBA);
        textureimage->loadFromImage(*t.get());

        t = Image::createFromFile("TestUnitImage", "assets/TestUnit.png", IRGBA);
        textureimage_TestUnit->loadFromImage(*t.get());

        t = Image::createFromFile("TestDotImage", "assets/dot.png", IRGBA);
        textureimage_dot->loadFromImage(*t.get());

        t = Image::createFromFile("heightmap", "assets/heightmap.png", IGRAY);
        textureimage_heightmap->loadFromImage(*t.get());

        t = Image::createFromFile("pretex", "assets/dune_pretex.png", IRGB);
        textureimage_dunepretex->loadFromImage(*t.get());

        std::shared_ptr<TextureImage> sandsoil(new TextureImage(
                "tile_sandsoil", TextureWrapMode::WRepeat));
        t = Image::createFromFile("sandsoil", "assets/tiles/sandsoil.png", IRGB);
        sandsoil->loadFromImage(*t.get());

        std::shared_ptr<TextureImage> stonewall2(new TextureImage(
                "tile_stonewall2", TextureWrapMode::WRepeat));
        t = Image::createFromFile("stonewall2", "assets/tiles/stonewall2.png", IRGB);
        stonewall2->loadFromImage(*t.get());

        std::shared_ptr<TextureImage> layer1_mask(new TextureImage("layer1"));
        t = Image::createFromFile("layer1", "assets/layer1.png", IGRAY);
        layer1_mask->loadFromImage(*t.get());

        std::shared_ptr<TextureImage> layer2_mask(new TextureImage("layer2"));
        t = Image::createFromFile("layer2", "assets/layer2.png", IGRAY);
        layer2_mask->loadFromImage(*t.get());

        using namespace Dune;

        terrain = new DuneTerrain(HPixel(8, 8), &renderer);
        terrain->setHeightmap(textureimage_heightmap);
        terrain->m_caches[0]->textures()[0]->loadFromTextureImage(*textureimage_dunepretex.get());
        terrain->m_caches[0]->initializeCanvas();
        auto tileset1 = std::make_shared<DuneTextureSet>();
        auto tileset2 = std::make_shared<DuneTextureSet>();
        tileset1->m_textures[0] = sandsoil;
        tileset2->m_textures[0] = stonewall2;

        auto layer1 = std::make_shared<DuneLayer>(tileset1, layer1_mask);
        auto layer2 = std::make_shared<DuneLayer>(tileset2, layer2_mask);
        layer2->uvscale = { 4.0, 0.0, 0.0 };

        auto chunk0 = terrain->chunkAt(0, 0);
        chunk0->appendLayer(layer1);
        chunk0->appendLayer(layer2);
    }

    {
        using namespace Howard;
        using namespace Howard::Hammer;

        HammerActorNodeStatic *ground = new
                HammerActorNodeStatic(&Foundation.rootNode(), { });
        std::unique_ptr<HammerHeightfield> field(new HammerHeightfield());
        field->setDataImage(terrain->m_scaled_height);
        field->generateHeightfield();
        PrimitiveHelper::attachHeightfieldToActor(*field, ground, terrain->heightfieldScale());
        ground->addToScene(&Foundation.mainPhysScene());
    }

    {
        JSAutoRequest req(Howard::Foundation.JSRuntime());
        JSAutoCompartment comp(Howard::Foundation.JSRuntime(), Howard::Foundation.JSGlobal());

        JS::RootedValue ret_preload(Howard::Foundation.JSRuntime());
        std::string buf = readfile("scripts/preload.js");
        JS_EvaluateScript(Howard::Foundation.JSRuntime(), Howard::Foundation.JSGlobal(), buf.c_str(),
            (unsigned int) buf.length(), "preload.js", 0, &ret_preload);
    }

    log("GLManager", L::Message) << "Context Initialized, entering loop (OpenGL " << glGetString(GL_VERSION) << ") ..." << rn;

    while (!glfwWindowShouldClose(window)) {
        // int width, height;
        // glfwGetFramebufferSize(window, &width, &height);
        // float ratio = width / (float) height;
        using namespace xoundation;
        using namespace Howard;

        {
            SpdRuntime& rt = Foundation.JSRuntime();
            JS::PersistentRootedObject& global = Foundation.JSGlobal();
            JSAutoRequest req(rt);
            JSAutoCompartment at_comp(rt, global);

            Foundation.eventQueues().queueUpdate()->invoke();

            Foundation.mainPhysScene().simulate(1.0 / 60.0f);
            Foundation.mainPhysScene().fetchSync();

            glfwPollEvents();
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            glm::vec3 offset = { -1, -1, 0 };
            offset *= 4;
            Howard::Dolly::IsometricCamera::instance->look_at += offset;
            Howard::Dolly::IsometricCamera::instance->pos += offset;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            glm::vec3 offset = { 1, 1, 0 };
            offset *= 4;
            Howard::Dolly::IsometricCamera::instance->look_at += offset;
            Howard::Dolly::IsometricCamera::instance->pos += offset;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            glm::vec3 offset = { -1, 1, 0 };
            offset *= 4;
            Howard::Dolly::IsometricCamera::instance->look_at += offset;
            Howard::Dolly::IsometricCamera::instance->pos += offset;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            glm::vec3 offset = { 1, -1, 0 };
            offset *= 4;
            Howard::Dolly::IsometricCamera::instance->look_at += offset;
            Howard::Dolly::IsometricCamera::instance->pos += offset;
        }

        Dolly::IsometricCamera::instance->update();
        std::shared_ptr<Stannum::RenderQueue> queue = std::make_shared<Stannum::RenderQueue>();
        Foundation.eventQueues().queuePaint()->setRenderQueue(queue).invoke();
        queue->pushDispatchCommand(std::make_shared<Howard::Dune::DispatchCommandDuneTerrain>(
                terrain, Howard::Dolly::IsometricCamera::instance));
        queue->pushDispatchCommand(std::make_shared<Stannum::DispatchCommandSprite>());
        renderer.render_dispatch(queue);

        glfwSwapBuffers(window);
    }

    renderer.destroy();
    delete &Howard::Foundation.JSRuntime();
    JS_ShutDown();
    glfwDestroyWindow(window);
    glfwTerminate();
    Howard::FSM::dispose();
    return 0;
}
