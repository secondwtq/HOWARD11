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

class RootNode : public HNode {

    public:
    RootNode();

    HowardNodeType node_typeid() const override { return HowardNodeType::NRootNode; }
    const char *node_type() const override { return RootNode::m_node_type; }
    static constexpr const char m_node_type[] = "RootNode";

    NodeManager *node_manager;

};

}

#endif // HOWARD11_ROOTNODE_HXX
