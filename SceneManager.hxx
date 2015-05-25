//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/05/24.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_SCENEMANAGER_HXX
#define HOWARD11_SCENEMANAGER_HXX

#include "HandleManager.hxx"
#include "Node.hxx"
#include "Asset.hxx"

namespace Howard {

class NodeManager : public HandleManager<Node> {

    public:
    static NodeManager *instance;

};

class AssetManager : public HandleManager<Asset> {

};

}

#endif // HOWARD11_SCENEMANAGER_HXX
