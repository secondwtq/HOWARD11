//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/05/24.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_HANDLEE_HXX
#define HOWARD11_HANDLEE_HXX

#include "Handle.hxx"

namespace Howard {

class NodeManager;

template <typename T>
using HandleObj = Handle<T, NodeManager>;

}

#endif // HOWARD11_HANDLEE_HXX
