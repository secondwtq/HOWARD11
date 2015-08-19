//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/28.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef ILLYBLAUNCHER_FACEREVENT_HXX
#define ILLYBLAUNCHER_FACEREVENT_HXX

#include <string>

#include "FacerEventKey.hxx"

namespace Facer {
    
    class InputEvent {
        
    public:
        
        enum EventType {
            NoType = 0,
            
            MouseMove,
            MousePress,
            MouseRelease,
            MouseWheel,
            
            KeyPress,
            KeyRelease,
            
            InputText
        };
        
        // WebKit the wise!
        enum MouseButton {
            NOBUTTON, LEFT, RIGHT, MIDDLE
        };
        
        enum ButtonState {
            RELEASED, PRESSED
        };
        
        struct MouseButtonState {
            ButtonState left = RELEASED;
            ButtonState right = RELEASED;
            ButtonState middle = RELEASED;
        };
        
        struct ModifierState {
            bool alt = false;
            bool shift = false;
            bool meta = false;
            bool control = false;
        };
        
        EventType type = EventType::NoType;
        MouseButton primaryButton = MouseButton::NOBUTTON;
        InputKeyCode keyCode = InputKeyCode::KNull;
        unsigned short cursorPosition[2] = { 0, 0 };
        float wheelDelta[2] = { .0f, .0f };
        MouseButtonState buttonState;
        ModifierState modifierState;
        std::string input { };
        long long timestamp = 0;
        
        inline bool isMouseCursorEvent() const {
            return this->type == MouseMove ||
                this->type == MousePress || this->type == MouseRelease;
        }
        
        // just helpers
        template <typename T>
        InputEvent& setCursorPosition(T xpos, T ypos) {
            this->cursorPosition[0] = static_cast<unsigned short>(xpos);
            this->cursorPosition[1] = static_cast<unsigned short>(ypos);
            return *this;
        }
        
        template <typename T>
        InputEvent& setWheelDelta(T xdel, T ydel) {
            this->wheelDelta[0] = static_cast<float>(xdel);
            this->wheelDelta[1] = static_cast<float>(ydel);
            return *this;
        }

        template <typename T>
        T getCursorPositionX() const {
            return this->cursorPosition[0]; }
        template <typename T>
        T getCursorPositionY() const {
            return this->cursorPosition[1]; }

        template <typename T>
        InputEvent& setCursorPositionX(T xpos) {
            this->cursorPosition[0] = xpos;
            return *this;
        }
        template <typename T>
        InputEvent& setCursorPositionY(T ypos) {
            this->cursorPosition[1] = ypos;
            return *this;
        }

        template <typename T>
        T getWheelDeltaX() const {
            return this->wheelDelta[0]; }
        template <typename T>
        T getWheelDeltaY() const {
            return this->wheelDelta[1]; }

        template <typename T>
        InputEvent& setWheelDeltaX(T xdel) {
            this->wheelDelta[0] = xdel;
            return *this;
        }
        template <typename T>
        InputEvent& setWheelDeltaY(T ydel) {
            this->wheelDelta[1] = ydel;
            return *this;
        }
    };
}

namespace Facer {
    namespace Port {
        InputEvent createEvent(InputEvent::EventType type);
    }
}

#endif // ILLYBLAUNCHER_FACEREVENT_HXX
