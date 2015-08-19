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

constexpr const char HowardBase::m_class_name[];
constexpr const char HNode::m_class_name[];
constexpr const char Asset::m_class_name[];

NodeManager *NodeManager::instance = nullptr;

constexpr const char EventListenerBase::m_class_name[];
constexpr const char EventListenerBase::m_listener_name[];

constexpr const char HNode::m_node_type[];
constexpr const char HNode::m_listener_name[];
constexpr const char RootNode::m_node_type[];
constexpr const char StannumSpriteNode::m_node_type[];
constexpr const char ScriptNodeBase::m_node_type[];

HNode::HNode(class RootNode *scene) : EventListenerBase(this), RTTIID(scene->node_manager->allocate_site()) {
    scene->node_manager->init_site(this->RTTIID, this); }

HNode::~HNode () {
    if (this->node_typeid() != HowardNodeType::NRootNode)
        this->root()->node_manager->clear_site(this->RTTIID);
    printf("Dwight - Node %u destructing ...\n", this->RTTIID);
}

RootNode *HNode::root() {
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

void HNode::invoke_event(HEvent::shared_ptr_t event)  {
    if (event->root() == nullptr) {
        event->set_root(this); }

    if (event->event_type() != EventType::EScriptEvent) {
        this->onEvent(event);
        if (event->stopped()) {
            return; }
        auto listeners = this->m_listeners.find(event->event_type());
        if (listeners != this->m_listeners.end()) {
            for (auto listener : listeners->second) {
                if (event->stopped()) {
                    return; }
                if (listener->enabled()) {
                    listener->onEvent(event); }
            }
        }
    } else {
        this->onScriptEvent(std::static_pointer_cast<ScriptEventBase>(event));
        if (event->stopped()) {
            return; }
        auto listeners = this->m_script_listeners.find(event->event_type_ext());
        if (listeners != this->m_script_listeners.end()) {
            for (auto listener : listeners->second) {
                if (event->stopped()) {
                    return; }
                if (listener->enabled()) {
                    listener->onScriptEvent(std::static_pointer_cast<ScriptEventBase>(event)); }
            }
        }
    }

    for (auto ch : m_children) {
        if (event->stopped()) {
            return; }
        ch->invoke_event(event);
    }
}

RootNode::RootNode() : HNode(HAS_FOUNDATION) {
    // a trick, currently we do not allow multiple RootNodes
    //  due to the existence of Handle mechanism.
    this->node_manager = NodeManager::instance = new NodeManager();

    size_t idx = node_manager->allocate_site();
    assert(idx == 0);
    this->RTTIID = idx;
    node_manager->init_site(idx, this);
}

}