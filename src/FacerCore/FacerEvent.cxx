//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/07/28.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "FacerEvent.hxx"

#include <time.h>

namespace Facer {
    namespace Port {
        
        InputEvent createEvent(InputEvent::EventType type) {
            InputEvent ret;
            ret.type = type;
            ret.timestamp = time(nullptr);
            return ret;
        }
        
    }
}
