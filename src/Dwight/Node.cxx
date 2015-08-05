//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/05/23.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#include "Node.hxx"
#include "RootNode.hxx"
#include "ScriptNode.hxx"
#include "StannumSpriteNode.hxx"
#include "ScriptEvent.hxx"

namespace Howard {

constexpr const char Node::m_node_type[];
constexpr const char RootNode::m_node_type[];
constexpr const char StannumSpriteNode::m_node_type[];
constexpr const char ScriptNode::m_node_type[];

Node::Node(class RootNode *scene) : RTTIID(scene->node_manager->allocate_site()) {
    scene->node_manager->init_site(this->RTTIID, this); }

Node::~Node () {
    if (!this->m_is_root)
        this->root()->node_manager->clear_site(this->RTTIID);
}

RootNode *Node::root() {
    if (this->node_typeid() != HowardNodeType::NRootNode) {
        if (this->has_parent()) {
            return this->parent()->root();
        } else {
            ASSERT_FOUNDATION();
            return nullptr;
        }
    } else {
        return reinterpret_cast<RootNode *>(this); }
}

void Node::invoke_event(Event::shared_ptr_t event)  {
    if (event->root() == nullptr) {
        event->set_root(this); }

    if (event->event_type() != EventType::EScriptEvent) {
        auto listeners = this->m_listeners.find(event->event_type());
        if (listeners != this->m_listeners.end()) {
            for (auto listener : listeners->second) {
                if (event->stopped()) {
                    return; }
                if (listener->enabled()) {
                    listener->on_event(event); }
            }
        }
    } else {
        auto listeners = this->m_script_listeners.find(event->event_type_ext());
        if (listeners != this->m_script_listeners.end()) {
            for (auto listener : listeners->second) {
                if (event->stopped()) {
                    return; }
                if (listener->enabled()) {
                    listener->on_event(event); }
            }
        }
    }

    for (auto ch : m_children) {
        if (event->stopped()) {
            return; }
        ch->invoke_event(event);
    }
}

RootNode::RootNode() : Node(HAS_FOUNDATION) {
    // a trick, currently we do not allow multiple RootNodes
    //  due to the existence of Handle mechanism.
    this->node_manager = NodeManager::instance = new NodeManager();

    size_t idx = node_manager->allocate_site();
    assert(idx == 0);
    this->RTTIID = idx;
    node_manager->init_site(idx, this);
}

}