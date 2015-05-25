//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/05/24.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_STANNUMTESTNODE_HXX
#define HOWARD11_STANNUMTESTNODE_HXX

#include <glm/glm.hpp>

#include "Node.hxx"

namespace Howard {

class StannumTestNode : public Node {

public:

    StannumTestNode (class RootNode *scene) : Node(scene) { }

    virtual HowardNodeType node_typeid() const { return HowardNodeType::NStannumTestNode; }
    virtual const char *node_type() const { return StannumTestNode::m_node_type; }

    static constexpr const char m_node_type[] = "StannumTestNode";

    size_t width = 96, height = 64;
    glm::vec2 location = glm::vec2(.0, .0);
    glm::vec3 color = glm::vec3(.0, 1.0, .0);

};

}

#endif // HOWARD11_STANNUMTESTNODE_HXX
