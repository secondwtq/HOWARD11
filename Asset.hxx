//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/05/23.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_ASSET_HXX
#define HOWARD11_ASSET_HXX

#include <string>

#include "Common.hxx"
#include "HowardBase.hxx"

namespace Howard {

class Asset : public HowardBase {

    public:

    HowardRTTIType WhatAmI() const override { return HowardRTTIType::TAsset; }

    const char *class_name() const override { return Asset::m_class_name; }
    virtual const char *asset_type() const { return Asset::m_asset_type; }

    static constexpr const char m_class_name[] = "Asset";
    static constexpr const char m_asset_type[] = "FoundationAsset";

    Asset(const char *name);
    Asset(const std::string& name) : Asset(name.c_str()) { }

    const std::string& name() { return m_name; }

    int AssetID = HO_HANDLE_NULL;

    private:

    std::string m_name;

};

}

#endif // HOWARD11_ASSET_HXX
