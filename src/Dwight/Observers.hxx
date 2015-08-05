//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love. 2015-08-04.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_OBSERVERS_HXX
#define HOWARD11_OBSERVERS_HXX

#include "Common.hxx"
#include "HowardBase.hxx"
#include "Debug.hxx"

#include "Event.hxx"

#include <map>
#include <unordered_map>

namespace Howard {

class Node;
class EventListener;
class Event;
class ScriptEvent;

class EventQueueGlobal : public HowardBase {
public:

    HowardRTTIType WhatAmI() const override { return HowardRTTIType::TEventQueueGlobal; }

    const char *class_name() const override { return EventQueueGlobal::m_class_name; }

    virtual const char *queue_name() const { return EventQueueGlobal::m_queue_name; }

    static constexpr const char m_class_name[] = "EventQueueGlobal";
    static constexpr const char m_queue_name[] = "QueueBase";

    bool inside(Node *node) const {
        auto i = m_nodes.find(node);
        return (i != m_nodes.end());
    }

    bool add(Node *node) {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i == m_nodes.end()) {
            m_nodes.insert({ node, true });
            return true;
        }
        return false;
    }

    bool remove(Node *node) {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i != m_nodes.end()) {
            m_nodes.erase(i);
            return true;
        }
        return false;
    }

    void invoke() {
        for (auto n : m_nodes) {
            if (n.second) {
                this->invoke_per_node(n.first);
            }
        }
    }

    bool enabled(Node *node) const {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i != m_nodes.end()) {
            return i->second;
        }
        return false;
    }

    bool disable(Node *node) {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i != m_nodes.end()) {
            i->second = false;
            return true;
        }
        return false;
    }

    bool enable(Node *node) {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i != m_nodes.end()) {
            i->second = true;
            return true;
        }
        return false;
    }

    virtual void invoke_per_node(Node *node) { }

private:

    std::map<Node *, bool> m_nodes;
};

class EventNotificationGlobal : public HowardBase {
public:

private:

//    std::unordered_map<EventType, std::map<Node *, bool>> m_nodes;
//    std::unordered_map<EventTypeExt, std::map<Node *, bool>> m_script_nodes;
};

}

#endif // HOWARD11_OBSERVERS_HXX
