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

#include <fstream>

using namespace Howard::FSMHelper;

void error_callback(int error, const char *desc) {
    printf("%s\n", desc); }

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == GLFW_KEY_UP) {
            glm::vec3 offset = { 1, 1, 0 };
            offset *= 32;
            Howard::Transform::IsometricCamera::instance->look_at += offset;
            Howard::Transform::IsometricCamera::instance->pos += offset;
        }
    }
}

void glfw_mousecb(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {

    }
}

void glfw_cursorcb(GLFWwindow *window, double x, double y) {

}

Howard::Stannum::StannumRenderer renderer;
Howard::Verdandi::Texture *texture;
Howard::RootNode *root;

void main_loop() {

    Howard::Transform::IsometricCamera::instance->update();

    Howard::Stannum::RenderQueue queue;
    root->on_paint_(&queue);

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
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Howard::Verdandi::gl_init();
    renderer.init();

    Howard::Verdandi::TextureImage *textureimage = new Howard::Verdandi::TextureImage("node");
    {
        Howard::Verdandi::Image image_t("load");
        std::ifstream file("assets/buildingbody.png", std::ios::binary);
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> buffer(size);
        if (file.read(buffer.data(), size)) {
            image_t.load_from_mem(reinterpret_cast<const Howard::RawDataT *>(buffer.data()), size);
        }
        textureimage->load(image_t);
    }
    texture = new Howard::Verdandi::Texture("node_tex", textureimage, { 133, 154 }, { 80, 71 });

    Howard::StannumSpriteNode *sprite = new Howard::StannumSpriteNode(root, texture);
    sprite->set_position({ 256, 0, 0 });
    sprite->attach_to({ &r });

    log("GLManager", L::Message) << "Context Initialized, entering loop (OpenGL " << glGetString(GL_VERSION) << ") ..." << rn;

    while (!glfwWindowShouldClose(window)) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float ratio = width / (float) height;

        main_loop();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    renderer.destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
    Howard::FSM::dispose();
}
