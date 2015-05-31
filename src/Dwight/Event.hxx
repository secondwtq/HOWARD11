//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-31
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_EVENT_HXX
#define HOWARD11_EVENT_HXX

namespace Howard {

enum EventType {
    Foundation,
    ScriptEvent
};

class Event {

    public:

    virtual EventType event_type() { return Foundation; }
    virtual int event_type_ext() { return EventType::Foundation; }

};

}

#endif // HOWARD11_EVENT_HXX
