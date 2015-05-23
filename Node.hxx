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

#include <vector>

namespace Howard {

class Node : public HowardBase {

    public:

    HowardRTTIType WhatAmI() const { return HowardRTTIType::TNode; }

    const char *class_name() const { return Node::m_class_name; }

    static constexpr const char m_class_name[] = "Node";

    size_t RTTIID = 0;

    void set_parent(Handle<Node> parent) {
        assert(!this->get_parent());
        this->m_parent = parent; }

    Handle<Node> get_parent() {
        return this->m_parent; }

    bool has_child(Handle<Node> child) {
        for (auto handle : m_children)
            if (handle == child)
                return true;
        return false;
    }

    bool has_parent() {
        return this->m_parent; }

    void add_child(Handle<Node> child) {
        if (!this->has_child(child)) {
            child->set_parent(this);
            this->m_children.push_back(child);
        }
    }

    void detach_child(Handle<Node> child) {
        ssize_t idx = HO_INVALIDX;
        for (size_t i = 0; i < m_children.size(); i++) {
            if (m_children[i] == child) {
                idx = i;
                break;
            }
        }

        if (idx != HO_INVALIDX) {
            m_children.erase(m_children.begin() + idx);
            child->set_parent(HO_HANDLE_NULL);
        }
    }

    void detach_from_parent() {
        if (this->has_parent()) {
            this->get_parent()->detach_child(this);
        }
    }

    private:

        Handle<Node> m_parent = nullptr;
        std::vector<Handle<Node>> m_children;

};

}

#endif // HOWARD11_NODE_HXX
