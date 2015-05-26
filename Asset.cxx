//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-05-26
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "Asset.hxx"

#include "FSMHelper.hxx"

#include "SceneManager.hxx"

namespace Howard {

using namespace FSMHelper;

constexpr const char Asset::m_asset_type[];

Asset::Asset(const char *name) : m_name(name) {
    if (AssetManager::instance->already_has(name)) {
        log("AssetMangaer", Error) << "Asset name duplicate: [" << name << "], " <<
                this->asset_type() << " & " << AssetManager::instance->named(name)->asset_type()
                << rn;
    } else {
        this->AssetID = AssetManager::instance->allocate_site();
        AssetManager::instance->init_site(this->AssetID, name, this);
    }
}

}