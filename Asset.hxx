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

#include "Common.hxx"
#include "HowardBase.hxx"

namespace Howard {

class Asset : public HowardBase {

    public:

    HowardRTTIType WhatAmI() { return HowardRTTIType::TAsset; }

    const char *class_name() { return Asset::m_class_name; }

    static constexpr const char m_class_name[] = "Asset";

};

}

#endif // HOWARD11_ASSET_HXX
