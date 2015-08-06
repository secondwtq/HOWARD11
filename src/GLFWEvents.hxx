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

#ifndef HOWARD11_GLFWEVENTS_HXX
#define HOWARD11_GLFWEVENTS_HXX

struct GLFWwindow;

void error_callback(int error, const char *desc);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void glfw_mousecb(GLFWwindow *window, int button, int action, int mods);
void glfw_cursorcb(GLFWwindow *window, double x, double y);
void glfw_fbsizecb(GLFWwindow *window, int width, int height);

#endif // HOWARD11_GLFWEVENTS_HXX
