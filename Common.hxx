//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love. 2015-05-23
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_COMMON_HXX
#define HOWARD11_COMMON_HXX

#include <stddef.h>

namespace Howard {

enum HowardRTTIType {
    TBase = 0,
    TNode = 1,
    THandle = 2,
    TAsset = 3,
};

}

#define HO_INVALIDX -1

#define HAS_FOUNDATION true
#define NO_FOUNDATION false

#define HO_ENDLINE "\n"

#endif // HOWARD11_COMMON_HXX
