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

namespace Howard {

constexpr const char Node::m_node_type[];
constexpr const char RootNode::m_node_type[];
constexpr const char StannumSpriteNode::m_node_type[];
constexpr const char ScriptNode::m_node_type[];

Node::Node(class RootNode *scene) : RTTIID(scene->node_manager->allocate_site()) {
    scene->node_manager->init_site(this->RTTIID, this); }

Node::~Node () {
    if (!this->m_is_root)
        this->get_root()->node_manager->clear_site(this->RTTIID);
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