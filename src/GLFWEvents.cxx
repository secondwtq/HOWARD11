//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-06
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "GLFWEvents.hxx"

#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <stdio.h>

#include "Misc/AtTheVeryBeginning.hxx"
#include "Misc/Transform.hxx"

#include "FacerCore/FacerEvent.hxx"
#include "FacerCore/FacerEventPortGLFW.hxx"
#include "Dwight/InputEvent.hxx"
#include "Dwight/Foundation.hxx"
#include "Dwight/Observers.hxx"

void error_callback(int error, const char *desc) {
    printf("%s\n", desc); }

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE); }

    Facer::InputEvent e = Facer::Port::GLFW::createEventKey(window, key, scancode, action, mods);
    std::shared_ptr<Howard::InputEvent> event(Howard::InputEvent::createShared(e));
    Howard::Foundation.eventQueues().queueKeyboard()->dispatch_event(event);
}

void glfw_mousecb(GLFWwindow *window, int button, int action, int mods) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventMouseButton(window, button, action, mods);
    std::shared_ptr<Howard::InputEvent> event(Howard::InputEvent::createShared(e));
    Howard::Foundation.eventQueues().queueMouseButton()->dispatch_event(event);
}

void glfw_cursorcb(GLFWwindow *window, double x, double y) {
    Facer::InputEvent e = Facer::Port::GLFW::createEventMouseMove(window, x, y);
    std::shared_ptr<Howard::InputEvent> event(Howard::InputEvent::createShared(e));
    Howard::Foundation.eventQueues().queueMouseMove()->dispatch_event(event);
}

void glfw_fbsizecb(GLFWwindow *window, int width, int height) {
    using namespace Howard::AtTheVeryBeginning;

    printf("Setting actual width %d, actual height %d ...\n", width, height);

    Setting<WindowSetting>::instance()->actual_width = width;
    Setting<WindowSetting>::instance()->actual_height = height;
}
