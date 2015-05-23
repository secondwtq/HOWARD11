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

namespace Howard {

class ScriptNode : public Node {

    public:

    HowardRTTIType WhatAmI() const { return HowardRTTIType::TScriptNode; }

    const char *class_name() const { return ScriptNode::m_class_name; }

    static constexpr const char m_class_name[] = "ScriptNode";

};

}

#endif // HOWARD11_SCRIPTNODE_HXX
