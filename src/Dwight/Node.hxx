//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-23
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_NODE_HXX
#define HOWARD11_NODE_HXX

#include "Common.hxx"
#include "HowardBase.hxx"
#include "Handle.hxx"
#include "Event.hxx"

#include "Debug.hxx"

#include "HandleManager.hxx"

#include <unordered_map>
#include <vector>
#include <set>
#include <memory>
#include <map>

#include <xoundation/spde.hxx>

#include "HandleE.hxx"
#include "../Misc/hash_fix.hxx"

namespace Howard {

enum HowardNodeType {

    NFoundation = 0,
    NRootNode = 1,
    NScriptNode = 2,
    NStannumSpriteNode = 3,

};

class Node;
class RootNode;
namespace Stannum {
    class RenderQueue; }

class EventListener;
class EventListenerScript;

//
// We'll guarantee several things for Howard::Event(s):
//
//  * the callback of parent node is called, prior to the child node
//
// and there is something undefined,
//  depending on something like the implementation of some STL classes
//
//  * the order of listeners of the same event and the same priority
//
class EventListener {
public:

    static constexpr const int DEFAULT_PRIORITY = 64;

    EventListener(Node *parent) :
            EventListener(parent, DEFAULT_PRIORITY) { }

    EventListener(Node *parent, int priority) :
            m_priority(priority), m_parent(parent) { }

    virtual ~EventListener() { }

    bool enabled() const { return this->m_enabled; }
    void set_enabled(bool enabled) { this->m_enabled = enabled; }
    void enable() { this->set_enabled(true); }
    void disable() { this->set_enabled(false); }

    // it should not exist, but for the script ...
    void set_listener_priority(int priority) { this->m_priority = priority; }
    int listener_priority() const { return m_priority; }
    Node *listener_parent() { return this->m_parent; }

    struct compare_ptr_priority {
        bool operator () (const EventListener * lhs, const EventListener * rhs) const {
            return (lhs->listener_priority()) < (rhs->listener_priority()); }
    };

    virtual void on_event(Event::shared_ptr_t event) { }

private:

    bool m_enabled = true;
    int m_priority = DEFAULT_PRIORITY;
    Node *m_parent = nullptr;
};

class Node : public EventListener, public xoundation::spd::intrusive_object<Node> {

    public:

    Node (class RootNode *scene);
    // Node (Node *scene) : Node(*scene) { };
    ~Node();

    Node (const Node&) = delete;
    Node& operator = (const Node&) = delete;

    static Node *create(RootNode *scene) { return new Node(scene); }

    HowardRTTIType WhatAmI() const { return HowardRTTIType::TNode; }
    const char *class_name() const { return Node::m_class_name; }

    // for ScriptNode, typeid is always NodeType::ScriptNode,
    //  but node_type is specified in Script.
    virtual HowardNodeType node_typeid() const { return HowardNodeType::NFoundation; };
    virtual const char *node_type() const { return Node::m_node_type; }

    static constexpr const char m_class_name[] = "Node";
    static constexpr const char m_node_type[] = "Foundation";

    int RTTIID = HO_HANDLE_NULL;

    // So how these function called?
    //
    // on_enter -> child->on_enter -> on_attach ->
    //  ...
    // -> on_detach -> child->on_enter -> on_exit
    virtual void on_attach() { }
    virtual void on_detach() { }
    virtual void on_enter() { }
    virtual void on_exit() { }

    // ATTENTION: this function is to be called
    //  in the Stannum thread and thus not to be overloaded
    //  by ScriptNode, it's used by Stannum nodes only.
    virtual void on_paint(Stannum::RenderQueue *queue) { }
    virtual void on_update() { }

    Node *parent() {
        return this->m_parent; }

    bool has_child(Node *child) const {
        for (auto handle : m_children)
            if (handle == child)
                return true;
        return false;
    }

    bool has_parent() const {
        return (this->m_parent != nullptr); }

    void add_child(Node *child) {
        if (!this->has_child(child)) {
            child->set_parent(this);
            this->m_children.push_back(child);
            child->on_enter_();
            child->on_attach();
        }
    }

    void detach_child(Node *child) {
        ssize_t idx = HO_INVALIDX;
        for (size_t i = 0; i < m_children.size(); i++) {
            if (m_children[i] == child) {
                idx = i;
                break;
            }
        }

        if (idx != HO_INVALIDX) {
            child->on_detach();
            child->on_exit_();
            m_children.erase(m_children.begin() + idx);
            child->set_parent(nullptr);
        }
    }

    // if the Node already has a parent
    //  you must detach first, then attach to another
    void attach_to(Node *parent) {
        ASSERT(!this->has_parent());
        if (!this->has_parent()) {
            parent->add_child(this); }
    }

    void detach_from_parent() {
        ASSERT(this->has_parent());
        if (this->has_parent()) {
            this->parent()->detach_child(this); }
    }

    RootNode *root();

    void on_paint_(Stannum::RenderQueue *queue) {
        for (auto ch : m_children) {
            ch->on_paint_(queue); }
        this->on_paint(queue);
    }

    void on_update_() {
        this->on_update();
        for (auto ch : m_children) {
            ch->on_update_(); }
    }

    EventListener *add_listener(EventType type, EventListener *listener) {
        ASSERT(type != EventType::EScriptEvent);
        ASSERT(listener->listener_parent() == this);

        this->m_listeners[type].insert(listener);
        return listener;
    }

    EventListener *add_listener(EventType type, EventTypeExt typext, EventListener *listener) {
        ASSERT(listener->listener_parent() == this);

        if (type != EventType::EScriptEvent) {
            this->m_listeners[type].insert(listener);
        } else {
            this->m_script_listeners[typext].insert(listener);
        }
        return listener;
    }

    EventListener *add_script_listener(EventTypeExt typext, EventListener *listener) {
        ASSERT(listener->listener_parent() == this);

        this->m_script_listeners[typext].insert(listener);
        return listener;
    }

    bool remove_listener(EventType type, const EventListener *listener) {
        ASSERT(listener);

        auto listeners_i = this->m_listeners.find(type);
        if (listeners_i != this->m_listeners.end()) {
            auto listeners = listeners_i->second;
            for (auto i = listeners.begin(); i != listeners.end(); ++i) {
                if (*i == listener) {
                    listeners.erase(i);
                    delete listener;
                    return true;
                }
            }
        }

        return false;
    }

    void invoke_event(Event::shared_ptr_t event);

// that's only for RootNode
protected:
    Node(bool any_found) : EventListener(this), m_is_root(true) { }

private:

    Node *m_parent = nullptr;
    std::vector<Node *> m_children;
    bool m_is_root = false;

    std::unordered_map<EventType, std::set<EventListener *, EventListener::compare_ptr_priority>,
            enum_hash<EventType>> m_listeners;
    std::unordered_map<EventTypeExt, std::set<EventListener *, EventListener::compare_ptr_priority>>
            m_script_listeners;

private:
    void on_enter_() {
        this->on_enter();
        for (auto ch : m_children) {
            ch->on_enter_(); }
    }

    void on_exit_() {
        for (auto ch : m_children) {
            ch->on_exit_(); }
        this->on_exit();
    }

    void set_parent(Node *parent) {
        assert(!this->parent());
        this->m_parent = parent; }

};

}

#include "RootNode.hxx"

#endif // HOWARD11_NODE_HXX
