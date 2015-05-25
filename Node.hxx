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

#include "Debug.hxx"

#include "HandleManager.hxx"

#include <vector>

#include "HandleE.hxx"

namespace Howard {

enum HowardNodeType {

    NFoundation = 0,
    NRootNode = 1,
    NScriptNode = 2,
    NStannumTestNode = 3,

};

class RootNode;

class StannumRenderQueue;

class Node : public HowardBase {

    public:

    Node (class RootNode *scene);
    Node (HandleObj<Node> scene) : Node(scene.get()) { };
    ~Node();

    Node (const Node&) = delete;
    Node& operator = (const Node&) = delete;

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
    virtual void on_paint(StannumRenderQueue *queue) { }

    void set_parent(HandleObj<Node> parent) {
        assert(!this->get_parent());
        this->m_parent = parent; }

    HandleObj<Node> get_parent() const {
        return this->m_parent; }

    bool has_child(HandleObj<Node> child) const {
        for (auto handle : m_children)
            if (handle == child)
                return true;
        return false;
    }

    bool has_parent() const {
        return this->m_parent; }

    void add_child(HandleObj<Node> child) {
        if (!this->has_child(child)) {
            child->set_parent(this);
            this->m_children.push_back(child);
            child->on_enter_();
            child->on_attach();
        }
    }

    void detach_child(HandleObj<Node> child) {
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
            child->set_parent(HO_HANDLE_NULL);
        }
    }

    void attach_to(HandleObj<Node> parent) {
        if (!this->has_parent()) {
            parent->add_child(this);
        }
    }

    void detach_from_parent() {
        if (this->has_parent()) {
            this->get_parent()->detach_child(this); }
    }

    RootNode *get_root() {
        if (this->node_typeid() != HowardNodeType::NRootNode) {
            if (this->has_parent()) {
                return this->get_parent()->get_root();
            } else {
                ASSERT_FOUNDATION(); }
        } else {
            return reinterpret_cast<RootNode *>(this); }
    }

    protected:

        // for RootNode
        Node(bool any_found) : m_is_root(true) { }

    private:

        HandleObj<Node> m_parent = nullptr;
        std::vector<HandleObj<Node>> m_children;
        bool m_is_root = false;

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

    void on_paint_(StannumRenderQueue *queue) {
        for (auto ch : m_children) {
            ch->on_paint_(queue); }
        this->on_paint(queue);
    }

};

}

#include "RootNode.hxx"

#endif // HOWARD11_NODE_HXX
