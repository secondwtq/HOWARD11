//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/05.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_FACEREVENTPORTGLFW_HXX
#define ILLYBLAUNCHER_FACEREVENTPORTGLFW_HXX

#include "FacerEvent.hxx"

class GLFWwindow;

namespace Facer {
namespace Port {
namespace GLFW {

InputEvent createEventMouseMove(GLFWwindow *window, int x, int y);
InputEvent createEventMouseButton(GLFWwindow *window, int button, int action, int mods);
InputEvent createEventMouseWheel(GLFWwindow *window, double xoffset, double yoffset);
InputEvent createEventKey(GLFWwindow *window, int key, int scancode, int action, int mods);
InputEvent createEventInputText(GLFWwindow *window, int charpoint);

InputEvent::ModifierState generateModifierState(int mods);
InputEvent::ModifierState generateModifierState(GLFWwindow *window);
InputKeyCode getKeyCode(int key);

}
}
}

#endif // ILLYBLAUNCHER_FACEREVENTPORTGLFW_HXX
