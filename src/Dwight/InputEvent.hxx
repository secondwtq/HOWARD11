//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-18
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_INPUTEVENT_HXX
#define HOWARD11_INPUTEVENT_HXX

#include "Event.hxx"

#include "../FacerCore/FacerEvent.hxx"

namespace Howard {

class InputEvent : public HEvent {
public:

    static std::shared_ptr<InputEvent> createShared(const Facer::InputEvent& facer_event) {
        return std::make_shared<InputEvent>(facer_event); }

    // ATTENTION: this is an exception: we allows the copy/assignment of some
    //  POD-like structure (like FacerEvent), but most of the cases,
    //  we just use references, and we have started employing more cool
    //  things like smart ptrs. maybe move semantics one day.
    InputEvent(const Facer::InputEvent& facer_event) : data(facer_event) { }

    EventType event_type() const override {
        return EventType::EInputEvent; }

    Facer::InputEvent& getData() {
        return data; }
    Facer::InputEvent data;
};

}

#endif // HOWARD11_INPUTEVENT_HXX
