//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/05/24.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_ROOTNODE_HXX
#define HOWARD11_ROOTNODE_HXX

#include "Node.hxx"
#include "SceneManager.hxx"

namespace Howard {

class RootNode : public Node {

    public:

    RootNode() : Node(HAS_FOUNDATION) {
        // a trick, currently we do not allow multiple RootNodes
        //  due to the existence of Handle mechanism.
        this->node_manager = NodeManager::instance = new NodeManager();

        size_t idx = node_manager->allocate_site();
        assert(idx == 0);
        this->RTTIID = idx;
        node_manager->init_site(idx, this);
    }

    HowardNodeType node_typeid() const { return HowardNodeType::NRootNode; }
    const char *node_type() const { return RootNode::m_node_type; }

    static constexpr const char m_node_type[] = "RootNode";

    NodeManager *node_manager;

};

}

#endif // HOWARD11_ROOTNODE_HXX
