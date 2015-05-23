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

void error_callback(int error, const char *desc) {
    printf("%s\n", desc); }

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main() {

    Howard::Node r;
    Howard::Handle<Howard::Node> h;
    printf("%s %s\n", h.class_name(), h.handle_type());

    assert(glfwInit());
    glfwSetErrorCallback(error_callback);

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow *window = glfwCreateWindow(800, 600, "HOWARD11 Engine Demo", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(-1);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    printf("Context Initialized, entering loop (OpenGL %s) ...\n", glGetString(GL_VERSION));

    while (!glfwWindowShouldClose(window)) {

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float ratio = width / (float) height;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
