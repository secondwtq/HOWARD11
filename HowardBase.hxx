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

#ifndef HOWARD11_HOWARDBASE_HXX
#define HOWARD11_HOWARDBASE_HXX

#include "Common.hxx"

namespace Howard {

    class HowardBase {

        public:

        HowardBase() { }

        virtual ~HowardBase() { }

        virtual HowardRTTIType WhatAmI() const { return HowardRTTIType::TBase; }

        const char *class_name() const { return HowardBase::m_class_name; }

        static constexpr const char m_class_name[] = "HowardName";

    };

}

#endif // HOWARD11_HOWARDBASE_HXX
