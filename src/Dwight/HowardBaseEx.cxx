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

#include "HowardBase.hxx"

#include "Common.hxx"
#include "Node.hxx"
#include "ScriptNode.hxx"
#include "Handle.hxx"
#include "Asset.hxx"

#include "HandleManager.hxx"
#include "SceneManager.hxx"

namespace Howard {

constexpr const char HowardBase::m_class_name[];
constexpr const char Node::m_class_name[];
constexpr const char Asset::m_class_name[];

NodeManager *NodeManager::instance = nullptr;

}