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

#include "../Misc/hash_fix.hxx"

namespace Howard {

enum HowardNodeType {

    NFoundation = 0,
    NRootNode = 1,
    NScriptNode = 2,
    NStannumSpriteNode = 3,

};

class HNode;
class RootNode;
namespace Stannum {
    class RenderQueue; }

class EventListenerBase;
class EventListenerScriptBase;

class ScriptEventBase;

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
class EventListenerBase : public HowardBase,
        std::enable_shared_from_this<EventListenerBase> {
public:

    static constexpr const int DEFAULT_PRIORITY = 64;

    EventListenerBase(HNode *parent) :
            EventListenerBase(parent, DEFAULT_PRIORITY) { }
    EventListenerBase(HNode *parent, int priority) :
            m_priority(priority), m_parent(parent) { }
    virtual ~EventListenerBase() { }

    HowardRTTIType WhatAmI() const override { return HowardRTTIType::TEventListener; }
    const char *class_name() const override { return EventListenerBase::m_class_name; }
    virtual const char *listenerName() const { return EventListenerBase::m_listener_name; }
    static constexpr const char m_class_name[] = "EventListenerBase";
    static constexpr const char m_listener_name[] = "EventListenerBase";

    bool enabled() const { return this->m_enabled; }
    void set_enabled(bool enabled) { this->m_enabled = enabled; }
    void enable() { this->set_enabled(true); }
    void disable() { this->set_enabled(false); }

    // it should not exist, but for the script ...
    void setPriority(int priority) { this->m_priority = priority; }
    int priority() const { return m_priority; }
    HNode *listenerParent() { return this->m_parent; }

    struct compare_ptr_priority {
        bool operator () (const std::shared_ptr<EventListenerBase> lhs,
                const std::shared_ptr<EventListenerBase> rhs) const {
            return (lhs->priority()) < (rhs->priority()); }
    };

    // TODO: should be abstract, but we provide an empty
    //  impl temporarily since mozjs needs its ctor avaliable
    virtual void onEvent(std::shared_ptr<HEvent> event) { }
    // given that ScriptEvent would be used muuuch more than other types
    //  and is more flexible, also requires extra cost for casting,
    //  we should add dedicated interfaces for ScriptEvent
    //  (we have already treat it as a special case
    //      inside the event dispatching code in Node)
    virtual void onScriptEvent(std::shared_ptr<ScriptEventBase> event) { }

protected:
    template <typename DowncastT>
    inline std::shared_ptr<DowncastT> get_shared() {
        return std::static_pointer_cast<DowncastT>(shared_from_this()); }

private:
    bool m_enabled = true;
    int m_priority = DEFAULT_PRIORITY;

    friend class HNode;
    void set_listener_parent(HNode *parent) {
        this->m_parent = nullptr; }
    HNode *m_parent = nullptr;

    // motivation:
    // today we changed the lifetime of EventListener (not the Node, which
    //  is directly derived from EventListener) to shared, originally we
    //  implicitly destruct it in Node::removeListener(). we want listeners
    //  decoupled from it's parent Nodes as much as possible.
    //
    // however, for the mozjs binding, we need to create a shared caster
    //  object when calling the event callbacks, for which we need to
    //  access the shared_ptr used to access 'this'.
    // STL <memory> provides std::enable_shared_from_this<T>, which
    //  can achieve the same functionality, and basically implemented
    //  with similar principle, but much more clear and safe, since
    //  it's just inside std::shared_ptr<T>.
    // ref: stackoverflow.com/questions/11711034/stdshared-ptr-of-this
    //
    // std::weak_ptr<EventListenerBase> shared_this;
};

class HNode : public EventListenerBase,
             public xoundation::spd::intrusive_object<HNode> {
    public:

    HNode (class RootNode *scene);
    // Node (Node *scene) : Node(*scene) { };
    ~HNode();

    HNode (const HNode&) = delete;
    HNode& operator = (const HNode&) = delete;

    static HNode *create(RootNode *scene) { return new HNode(scene); }

    HowardRTTIType WhatAmI() const override { return HowardRTTIType::TNode; }
    const char *class_name() const override { return HNode::m_class_name; }
    const char *listenerName() const override { return HNode::m_listener_name; }
    // for ScriptNode, typeid is always NodeType::ScriptNode,
    //  but node_type is specified in Script.
    virtual HowardNodeType node_typeid() const { return HowardNodeType::NFoundation; };
    virtual const char *node_type() const { return HNode::m_node_type; }

    static constexpr const char m_class_name[] = "HNode";
    static constexpr const char m_listener_name[] = "NodeListener";
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

    // ATTENTION: this function is to be called in the
    //  Stannum thread, and thus not to be overloaded
    //  by ScriptNode, it's used by Stannum nodes only.
    virtual void onPaint(Stannum::RenderQueue *queue) { }
    virtual void onUpdate() { }

    void onEvent(HEvent::shared_ptr_t event) override { }
    void onScriptEvent(std::shared_ptr<ScriptEventBase> event) override { }

    HNode *parent() {
        return this->m_parent; }

    bool has_child(HNode *child) const {
        for (auto handle : m_children)
            if (handle == child)
                return true;
        return false;
    }

    bool has_parent() const {
        return (this->m_parent != nullptr); }

    void add_child(HNode *child) {
        if (!this->has_child(child)) {
            child->set_parent(this);
            this->m_children.push_back(child);
            child->on_enter_();
            child->on_attach();
        }
    }

    void detach_child(HNode *child) {
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
    void attach_to(HNode *parent) {
        ASSERT(!this->has_parent());
        if (!this->has_parent()) {
            parent->add_child(this); }
    }

    void detach_from_parent() {
        ASSERT(this->has_parent());
        if (this->has_parent()) {
            this->parent()->detach_child(this); }
    }

    size_t get_length() const {
        return this->m_children.size(); }

    HNode *child(size_t idx) {
        return this->m_children[idx]; }

    RootNode *root();

    std::shared_ptr<EventListenerBase> addListener(EventType type, std::shared_ptr<EventListenerBase> listener) {
        ASSERT(type != EventType::EScriptEvent);
        if (!listener->listenerParent()) {
            listener->m_parent = this; }
        ASSERT(listener->listenerParent() == this);

        this->m_listeners[type].insert(listener);
        return listener;
    }

    std::shared_ptr<EventListenerBase> addListener(EventType type,
            EventTypeExt typext,std::shared_ptr<EventListenerBase>
    listener) {
        if (!listener->listenerParent()) {
            listener->m_parent = this; }
        ASSERT(listener->listenerParent() == this);

        if (type != EventType::EScriptEvent) {
            m_listeners[type].insert(listener);
        } else { m_script_listeners[typext].insert(listener); }
        return listener;
    }

    std::shared_ptr<EventListenerBase> addScriptListener(EventTypeExt typext,
            std::shared_ptr<EventListenerBase> listener) {
        if (!listener->listenerParent()) {
            listener->m_parent = this; }
        ASSERT(listener->listenerParent() == this);

        m_script_listeners[typext].insert(listener);
        return listener;
    }

    // TODO: removeListener() for ScriptEvent
    bool removeListener(EventType type, const std::shared_ptr<EventListenerBase> listener) {
        ASSERT(listener);

        auto listeners_i = this->m_listeners.find(type);
        if (listeners_i != this->m_listeners.end()) {
            auto listeners = listeners_i->second;
            for (auto i = listeners.begin(); i != listeners.end(); ++i) {
                if (*i == listener) {
                    (*i)->set_listener_parent(nullptr);
                    listeners.erase(i);
                    // delete listener;
                    return true;
                }
            }
        }

        return false;
    }

    void invoke_event(HEvent::shared_ptr_t event);

// that's only for RootNode
protected:
    HNode(bool any_found) : EventListenerBase(this) { }

private:

    HNode *m_parent = nullptr;
    std::vector<HNode *> m_children;

    std::unordered_map<EventType,
            std::set<std::shared_ptr<EventListenerBase>,
            EventListenerBase::compare_ptr_priority>,
            enum_hash<EventType>> m_listeners;
    std::unordered_map<EventTypeExt,
            std::set<std::shared_ptr<EventListenerBase>,
            EventListenerBase::compare_ptr_priority>>
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

    void set_parent(HNode *parent) {
        assert(!this->parent());
        this->m_parent = parent; }
};

}

#endif // HOWARD11_NODE_HXX
