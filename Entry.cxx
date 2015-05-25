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

#include "Debug.hxx"

#include <stdio.h>
#include <stdlib.h>

#include "Node.hxx"
#include "Handle.hxx"
#include "RootNode.hxx"

#include "GLFoundation.hxx"
#include "Stannum.hxx"
#include "AtTheVeryBeginning.hxx"

#include "FSM.hxx"

void error_callback(int error, const char *desc) {
    printf("%s\n", desc); }

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

Howard::Stannum::StannumRenderer renderer;

void main_loop() {

    Howard::Stannum::StannumDataTest data;
    data.set_position_and_size({ 0, 0 }, { 200, 150 });
    data.set_color({ 1.0, 0.0, 0.0, 1.0 });

    Howard::Stannum::StannumRenderQueue queue;
    queue.push(new Howard::Stannum::StannumCommandTest(&data));

    renderer.render_dispatch(queue);
}

void glfw_fbsizecb(GLFWwindow *window, int width, int height) {
    using namespace Howard::AtTheVeryBeginning;

    printf("Setting actual width %d, actual height %d ...\n", width, height);

    Setting<WindowSetting>::instance()->actual_width = width;
    Setting<WindowSetting>::instance()->actual_height = height;
}

int main() {
    Howard::FSM::init();

    Howard::AtTheVeryBeginning::SettingGlobal::instance()->load("config.rc");

    Howard::RootNode root;
    Howard::Node r(&root);
    r.attach_to({ &root });

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
    glfwSetFramebufferSizeCallback(window, glfw_fbsizecb);
    glfwMakeContextCurrent(window);

    Howard::Verdandi::gl_init();
    renderer.init();

    printf("Context Initialized, entering loop (OpenGL %s) ...\n", glGetString(GL_VERSION));

    while (!glfwWindowShouldClose(window)) {

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float ratio = width / (float) height;

        main_loop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
