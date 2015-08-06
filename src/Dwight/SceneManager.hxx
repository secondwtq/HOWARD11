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

#include <string>
#include <unordered_map>

namespace Howard {

class NodeManager : public HandleManager<Node> {

    public:
    static NodeManager *instance;

};

class AssetManager : public HandleManager<Asset> {

    public:
    static AssetManager *instance;

    bool already_has(const char *name) {
        return (this->m_namemap.find(name) != m_namemap.end()); }

    Asset *named(const char *name) {
        return this->m_namemap.at(name); }

    Asset *named(const std::string& name) {
        return this->m_namemap.at(name.c_str()); }

    inline void init_site(std::size_t id, const char *name, Asset *ptr) {
        HandleManager<Asset>::init_site(id, ptr);
        this->m_namemap[name] = ptr;
    }

    private:

    std::unordered_map<std::string, Asset *> m_namemap;

};

}

#endif // HOWARD11_SCENEMANAGER_HXX
