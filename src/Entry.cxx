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
#include "Dwight/Handle.hxx"
#include "Dwight/RootNode.hxx"
#include "Dwight/StannumSpriteNode.hxx"

#include "Verdandi/GLFoundation.hxx"
#include "Stannum/Stannum.hxx"
#include "Stannum/StannumSprite.hxx"
#include "Misc/AtTheVeryBeginning.hxx"
#include "Misc/Transform.hxx"

#include "FSM/FSM.hxx"
#include "FSM/FSMHelper.hxx"

#include "GLFWEvents.hxx"
#include "JSFunctions.hxx"

#include "Dwight/ScriptNode.hxx"
#include "Dwight/ScriptEvent.hxx"

#include <string>
#include <memory>

using namespace Howard::FSMHelper;

Howard::Stannum::StannumRenderer renderer;
Howard::Verdandi::Texture *texture;
Howard::RootNode *root;

#define SPD_DEF(target) decltype(target), (target)

void main_loop() {

    Howard::Transform::IsometricCamera::instance->update();

    Howard::Stannum::RenderQueue queue;
    root->on_paint_(&queue);

    renderer.render_dispatch(queue);
}

using namespace xoundation;
xoundation::SpdRuntime *jsruntime = nullptr;
JS::PersistentRootedObject *jsglobal = nullptr;

inline Howard::RootNode *getRootNode() {
    return root; }

int main() {
    Howard::FSM::init();
    Howard::AtTheVeryBeginning::SettingGlobal::instance()->load("config.rc");
    jsruntime = new xoundation::SpdRuntime();
    {
        JSAutoRequest req(*jsruntime);
        JS::RootedObject global(*jsruntime);
        global = JS_NewGlobalObject(*jsruntime, &xoundation::cls_global, nullptr, JS::DontFireOnNewGlobalHook);
        if (!global) {
            return 1; }
        jsglobal = new JS::PersistentRootedObject(*jsruntime, global);

        JSAutoCompartment at_comp(*jsruntime, global);
        if (!JS_InitStandardClasses(*jsruntime, global)) {
            return 1; }
        JS_DefineFunction(*jsruntime, global, "print", js_print, 0,
            JSPROP_ENUMERATE | JSPROP_PERMANENT | JSFUN_STUB_GSOPS);

        using namespace Howard;

        spd::enumeration<HowardRTTIType>().define(*jsruntime, global, "HowardRTTIType")
                .enumerator<HowardRTTIType::TBase>("TBase")
                .enumerator<HowardRTTIType::TNode>("TNode")
                .enumerator<HowardRTTIType::THandle>("THandle")
                .enumerator<HowardRTTIType::TAsset>("TAsset")
                .enumerator<HowardRTTIType::TEventQueueGlobal>("TEventQueueGlobal")
                .enumerator<HowardRTTIType::TEventNotificationCenter>("TEventNotificationCenter");

        spd::enumeration<HowardNodeType>().define(*jsruntime, global, "HowardNodeType")
                .enumerator<HowardNodeType::NFoundation>("NFoundation")
                .enumerator<HowardNodeType::NRootNode>("NRootNode")
                .enumerator<HowardNodeType::NScriptNode>("NScriptNode")
                .enumerator<HowardNodeType::NStannumSpriteNode>("NStannumSpriteNode");

        spd::class_info<HowardBase>::inst_wrapper::set(new spd::class_info<HowardBase>(*jsruntime, "HowardBase"));
        klass<HowardBase>().define<>(global)
                .method<SPD_DEF(&HowardBase::WhatAmI)>("WhatAmI")
                .method<decltype(&HowardBase::class_name), &HowardBase::class_name>("class_name");

        spd::class_info<Asset>::inst_wrapper::set(new spd::class_info<Asset>(*jsruntime, "Asset"));
        klass<Asset>().inherits<HowardBase, const std::string&>(global)
                .method<SPD_DEF(&Asset::asset_type)>("asset_type")
                .method<SPD_DEF(&Asset::name)>("name");

        spd::class_info<AssetManager>::inst_wrapper::set(new spd::class_info<AssetManager>(*jsruntime, "AssetManager"));
        klass<AssetManager>().define<>(global)
                .method<Asset *(AssetManager::*)(const std::string&), &AssetManager::named>("named")
                .static_prop<AssetManager *, &AssetManager::instance>("instance");

        spd::class_info<Verdandi::TextureImage>::inst_wrapper::set(
                new spd::class_info<Verdandi::TextureImage>(*jsruntime, "TextureImage"));
        klass<Verdandi::TextureImage>().inherits<Asset, const std::string&>(global);

        spd::class_info<Verdandi::Texture>::inst_wrapper::set(
                new spd::class_info<Verdandi::Texture>(*jsruntime, "Texture"));
        klass<Verdandi::Texture>().inherits<Asset, const std::string&, Verdandi::TextureImage *>(global)
                .static_func<SPD_DEF(Verdandi::Texture::createWithEntireImage)>("createWithEntireImage");

        spd::class_info<EventListener>::inst_wrapper::set(new spd::class_info<EventListener>(*jsruntime, "EventListener"));
        klass<EventListener>().inherits<HowardBase, Node *>(global);

        spd::class_info<Node>::inst_wrapper::set(new spd::class_info<Node>(*jsruntime, "Node"));
        klass<Node>().inherits<EventListener, RootNode *>(global)
                .static_func<decltype(Node::create), Node::create>("create")
                .method<SPD_DEF(&Node::node_typeid)>("node_typeid")
                .method<SPD_DEF(&Node::node_type)>("node_type")
                .method<SPD_DEF(&Node::parent)>("parent")
                .method<SPD_DEF(&Node::has_parent)>("has_parent")
                .method<SPD_DEF(&Node::has_child)>("has_child")
                .method<SPD_DEF(&Node::add_child)>("add_child")
                .method<SPD_DEF(&Node::detach_child)>("detach_child")
                .method<SPD_DEF(&Node::attach_to)>("attach_to")
                .method<SPD_DEF(&Node::detach_from_parent)>("detach_from_parent")
                .method<SPD_DEF(&Node::root)>("root");

        spd::class_info<RootNode>::inst_wrapper::set(new spd::class_info<RootNode>(*jsruntime, "RootNode"));
        klass<RootNode>().inherits<Node>(global);

        spd::class_info<StannumSpriteNode>::inst_wrapper::set(
                new spd::class_info<StannumSpriteNode>(*jsruntime, "StannumSpriteNode"));
        klass<StannumSpriteNode>().inherits<Node, RootNode *, Verdandi::Texture *>(global)
                .static_func<SPD_DEF(StannumSpriteNode::create)>("create")
                .method<SPD_DEF(&StannumSpriteNode::set_position)>("set_position");

        spd::class_info<Event>::inst_wrapper::set(new spd::class_info<Event>(*jsruntime, "Event"));
        klass<Event>().define(global)
                .static_func<SPD_DEF(Event::create_shared)>("create_shared")
                .method<SPD_DEF(&Event::event_type_ext)>("event_type_ext")
                .method<SPD_DEF(&Event::root)>("root")
                .method<SPD_DEF(&Event::stop_propagation)>("stop_propagation")
                .method<SPD_DEF(&Event::stopped)>("stopped");

        spd::class_info<HCoord>::inst_wrapper::set(new spd::class_info<HCoord>(*jsruntime, "HCoord"));
        klass<HCoord>().define<int, int, int>(global)
                .property<int, &HCoord::x>("x")
                .property<int, &HCoord::y>("y")
                .property<int, &HCoord::z>("z");

        spd::class_info<HPoint>::inst_wrapper::set(new spd::class_info<HPoint>(*jsruntime, "HPoint"));
        klass<HPoint>().define(global)
                .property<short, &HPoint::x>("x")
                .property<short, &HPoint::y>("y");

        JS_DefineFunction(*jsruntime, global, "getRootNode",
                          spd::function_callback_wrapper<decltype(getRootNode), getRootNode>::callback,
                          0, JSPROP_ENUMERATE | JSPROP_PERMANENT | JSFUN_STUB_GSOPS);
    }

    root = new Howard::RootNode();
    Howard::Node r(root);
    r.attach_to({ root });

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
        JSAutoRequest req(*jsruntime);
        JSAutoCompartment comp(*jsruntime, *jsglobal);

        JS::RootedValue ret_preload(*jsruntime);
        std::string buf = readfile("preload.js");
        JS_EvaluateScript(*jsruntime, *jsglobal, buf.c_str(),
            (unsigned int) buf.length(), "preload.js", 0, &ret_preload);
    }

    Howard::StannumSpriteNode *sprite = new Howard::StannumSpriteNode(root, texture);
    sprite->set_position({ 512, 256, 0 });
    sprite->attach_to({ &r });

    log("GLManager", L::Message) << "Context Initialized, entering loop (OpenGL " << glGetString(GL_VERSION) << ") ..." << rn;

    while (!glfwWindowShouldClose(window)) {
        // int width, height;
        // glfwGetFramebufferSize(window, &width, &height);
        // float ratio = width / (float) height;

        main_loop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    renderer.destroy();
    JS_ShutDown();
    glfwDestroyWindow(window);
    glfwTerminate();
    Howard::FSM::dispose();
    return 0;
}
