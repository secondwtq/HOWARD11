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

#ifndef HOWARD11_SCRIPTEVENT_HXX
#define HOWARD11_SCRIPTEVENT_HXX

#include "Event.hxx"

namespace Howard {

class ScriptEvent : public Event {

    public:

    EventType event_type() override { return ScriptEvent; }

};

}

#endif // HOWARD11_SCRIPTEVENT_HXX
