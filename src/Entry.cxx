//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love. 2015-05-22.
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

#include <string>
#include <memory>
#include <vector>

using namespace Howard::FSMHelper;

namespace Howard {
FoundationInstance Foundation;
}

Howard::Stannum::StannumRenderer renderer;
Howard::Verdandi::Texture *texture;

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
                .static_func<SPD_DEF(Verdandi::Texture::createWithEntireImage)>("createWithEntireImage");

        class_info<EventListenerBase>::inst_wrapper::set(new
                class_info<EventListenerBase>(rt, "EventListenerBase"));
        klass<EventListenerBase>().inherits<HowardBase>(global, argpack<HNode *>());

        // class EventListenerScript : EventListenerBase - ScriptEvent.hxx
        class_info<EventListenerScriptBase>::inst_wrapper::set(new
                class_info<EventListenerScriptBase>(rt, "EventListenerScriptBase"));
        klass<EventListenerScriptBase>().inherits<EventListenerBase>(global,
                argpack<HNode *, const std::string&, context_reference>())
                .property<JS::PersistentRootedObject, &EventListenerScriptBase::scriptObject>("data");

        // struct EventListenerScriptData - ScriptEvent.hxx
        class_info<EventListenerScriptData>::inst_wrapper::set(new
                class_info<EventListenerScriptData>(rt, "EventListenerScriptData"));
        klass<EventListenerScriptData>().define(global, argpack<std::shared_ptr<EventListenerScriptBase>>())
                .accessor<std::shared_ptr<EventListenerScriptBase>, &EventListenerScriptData::get_listener>("listener");

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
                .method<SPD_DEF(&HNode::invoke_event)>("invoke_event");

        class_info<RootNode>::inst_wrapper::set(new spd::class_info<RootNode>(rt, "RootNode"));
        klass<RootNode>().inherits<HNode>(global, spd::argpack<>());

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
                .accessor<AssetManager&, &FoundationInstance::assetManager>("assetManager");
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
                .accessor<HCoord, &StannumSpriteNode::position>("position")
                .method<SPD_DEF(&StannumSpriteNode::set_position)>("set_position");

        // class Event - Event.hxx
        class_info<HEvent>::inst_wrapper::set(new spd::class_info<HEvent>(rt, "HEvent"));
        klass<HEvent>().define(global)
                .static_func<SPD_DEF(HEvent::createShared)>("createShared")
                .method<SPD_DEF(&HEvent::event_type)>("event_type")
                .method<SPD_DEF(&HEvent::event_type_ext)>("event_type_ext")
                .method<SPD_DEF(&HEvent::root)>("root")
                .method<SPD_DEF(&HEvent::stop_propagation)>("stop_propagation")
                .method<SPD_DEF(&HEvent::stopped)>("stopped");

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
    }

    Howard::Foundation.setRootNode(new Howard::RootNode());
    Howard::Foundation.setAssetManager(new Howard::AssetManager());
    Howard::Foundation.setEventQueues(new Howard::EventQueueManager());
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

    Howard::Verdandi::TextureImage *textureimage = new Howard::Verdandi::TextureImage("node");
    {
        Howard::Verdandi::Image image_t("load");
        std::string buf = readfile("assets/buildingbody.png");
        image_t.load_from_mem(reinterpret_cast<const Howard::RawDataT *>(buf.c_str()), buf.length());
        textureimage->load(image_t);
    }
    texture = new Howard::Verdandi::Texture("node_tex", textureimage, { 133, 154 }, { 80, 71 });

    {
        JSAutoRequest req(Howard::Foundation.JSRuntime());
        JSAutoCompartment comp(Howard::Foundation.JSRuntime(), Howard::Foundation.JSGlobal());

        JS::RootedValue ret_preload(Howard::Foundation.JSRuntime());
        std::string buf = readfile("scripts/preload.js");
        JS_EvaluateScript(Howard::Foundation.JSRuntime(), Howard::Foundation.JSGlobal(), buf.c_str(),
            (unsigned int) buf.length(), "preload.js", 0, &ret_preload);
    }

    Howard::StannumSpriteNode *sprite = new Howard::StannumSpriteNode(&Howard::Foundation.rootNode(), texture);
    sprite->set_position({ 512, 256, 0 });
    sprite->attach_to({ &r });
    Howard::Foundation.eventQueues().queuePaint()->add(sprite);

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

            glfwPollEvents();
        }

        Transform::IsometricCamera::instance->update();
        std::shared_ptr<Stannum::RenderQueue> queue(new Stannum::RenderQueue());
        Foundation.eventQueues().queuePaint()->setRenderQueue(queue).invoke();

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
