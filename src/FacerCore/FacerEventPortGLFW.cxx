//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/08/05.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "FacerEvent.hxx"
#include "FacerEventPortGLFW.hxx"

#include <GLFW/glfw3.h>

#include <locale>
#include <codecvt>

namespace Facer {
namespace Port {
namespace GLFW {

InputEvent::ModifierState generateModifierState(int mods) {
    InputEvent::ModifierState ret;

    ret.alt = (mods & GLFW_MOD_ALT) != 0;
    ret.shift = (mods & GLFW_MOD_SHIFT) != 0;
    ret.meta = (mods & GLFW_MOD_SUPER) != 0;
    ret.control = (mods & GLFW_MOD_CONTROL) != 0;

    return ret;
}

InputEvent::ModifierState generateModifierState(GLFWwindow *window) {
    InputEvent::ModifierState ret;

    // TODO: we lost the flexibility of left/right modifier keys
#define _FACER_PRESSED_(name) ((glfwGetKey(window, GLFW_KEY_LEFT_##name) == GLFW_PRESS) || \
    (glfwGetKey(window, GLFW_KEY_RIGHT_##name) == GLFW_PRESS))

    ret.alt = _FACER_PRESSED_(ALT);
    ret.shift = _FACER_PRESSED_(SHIFT);
    ret.meta = _FACER_PRESSED_(SUPER);
    ret.control = _FACER_PRESSED_(CONTROL);

#undef _FACER_PRESSED_

    return ret;
}

InputEvent createEventMouseMove(GLFWwindow *window, int x, int y) {
    InputEvent ret = createEvent(InputEvent::MouseMove);
    ret.setCursorPosition(x, y);
    ret.modifierState = generateModifierState(window);
    return ret;
}

InputEvent createEventMouseButton(GLFWwindow *window, int button, int action, int mods) {
    InputEvent::EventType type = InputEvent::NoType;
    if (action == GLFW_PRESS) {
        type = InputEvent::MousePress;
    } else if (action == GLFW_RELEASE) {
        type = InputEvent::MouseRelease; }
    InputEvent ret = createEvent(type);

    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);
    ret.setCursorPosition(xpos, ypos);
    ret.modifierState = generateModifierState(mods);

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            ret.primaryButton = InputEvent::LEFT;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            ret.primaryButton = InputEvent::RIGHT;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            ret.primaryButton = InputEvent::MIDDLE;
            break;
        default:
            break;
    }

    return ret;
}

InputEvent createEventMouseWheel(GLFWwindow *window, double xoffset, double yoffset) {
    InputEvent ret = createEvent(InputEvent::MouseWheel);
    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);
    ret.setCursorPosition(xpos, ypos);
    ret.modifierState = generateModifierState(window);

    return ret;
}

InputEvent createEventInputText(GLFWwindow *window, int charpoint) {
    InputEvent ret = createEvent(InputEvent::InputText);

    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);
    ret.setCursorPosition(xpos, ypos);
    ret.modifierState = generateModifierState(window);

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cv;
    std::string r = cv.to_bytes(charpoint);
    ret.input = r;

    return ret;
}

InputKeyCode getKeyCode(int key) {

    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
        return static_cast<InputKeyCode>(KLetterCapA + key - GLFW_KEY_A); }

    if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
        return static_cast<InputKeyCode>(KNumber0 + key - GLFW_KEY_0); }

#define _FACER_DEFKEY_(GLFW_KEY, FACER_KEY) case GLFW_KEY: return FACER_KEY;

    switch (key) {
        _FACER_DEFKEY_(GLFW_KEY_SPACE, KSpace);
        _FACER_DEFKEY_(GLFW_KEY_APOSTROPHE, KQuoteSingle);
        _FACER_DEFKEY_(GLFW_KEY_COMMA, KComma)
        _FACER_DEFKEY_(GLFW_KEY_MINUS, KHyphen);
        _FACER_DEFKEY_(GLFW_KEY_PERIOD, KPeriod);
        _FACER_DEFKEY_(GLFW_KEY_SLASH, KSlash);
        _FACER_DEFKEY_(GLFW_KEY_SEMICOLON, KSemicolon);
        _FACER_DEFKEY_(GLFW_KEY_EQUAL, KEqual);
        _FACER_DEFKEY_(GLFW_KEY_LEFT_BRACKET, KBracketLeft);
        _FACER_DEFKEY_(GLFW_KEY_BACKSLASH, KBackslash);
        _FACER_DEFKEY_(GLFW_KEY_RIGHT_BRACKET, KBracketRight);
        _FACER_DEFKEY_(GLFW_KEY_GRAVE_ACCENT, KGraveAccent);

        _FACER_DEFKEY_(GLFW_KEY_ESCAPE, KEscape);
        _FACER_DEFKEY_(GLFW_KEY_ENTER, KReturn);
        _FACER_DEFKEY_(GLFW_KEY_TAB, KTab);
        _FACER_DEFKEY_(GLFW_KEY_BACKSPACE, KBackspace);
        _FACER_DEFKEY_(GLFW_KEY_DELETE, KDelete);

        default:
            return KNull;
    }

#undef _FACER_DEFKEY_
}

InputEvent createEventKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    InputEvent::EventType type = InputEvent::NoType;
    if (action == GLFW_PRESS) {
        type = InputEvent::KeyPress;
    } else if (action == GLFW_RELEASE) {
        type = InputEvent::KeyRelease; }
    InputEvent ret = createEvent(type);

    double xpos = 0, ypos = 0;
    glfwGetCursorPos(window, &xpos, &ypos);
    ret.setCursorPosition(xpos, ypos);
    ret.modifierState = generateModifierState(mods);

    ret.keyCode = getKeyCode(key);
    if (ret.keyCode == KNull) {
        printf("FacerInputEvent (port source GLFW): Unknown source keycode %d!\n", key); }

    return ret;
}

}
}
}
