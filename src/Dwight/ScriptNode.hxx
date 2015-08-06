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

#ifndef HOWARD11_SCRIPTNODE_HXX
#define HOWARD11_SCRIPTNODE_HXX

#include "Common.hxx"
#include "Node.hxx"
#include "HandleE.hxx"

#include "../thirdpt/mozjs.hxx"

namespace Howard {

class ScriptNode : public Node {

public:

    ScriptNode (RootNode *scene) : Node(scene) { }
    ScriptNode (HandleObj<Node> scene) : Node(scene) { }

    static ScriptNode *create(RootNode *scene) { return new ScriptNode(scene); }

    HowardNodeType node_typeid() const override { return HowardNodeType::NScriptNode; }
    const char *node_type() const override { return ScriptNode::m_node_type; }

    static constexpr const char m_node_type[] = "ScriptNode";

    virtual void on_update() override { }
    virtual void on_event(Event::shared_ptr_t event) override { }

};

}

#endif // HOWARD11_SCRIPTNODE_HXX
