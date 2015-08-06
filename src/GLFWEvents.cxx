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

void glfw_fbsizecb(GLFWwindow *window, int width, int height) {
    using namespace Howard::AtTheVeryBeginning;

    printf("Setting actual width %d, actual height %d ...\n", width, height);

    Setting<WindowSetting>::instance()->actual_width = width;
    Setting<WindowSetting>::instance()->actual_height = height;
}
