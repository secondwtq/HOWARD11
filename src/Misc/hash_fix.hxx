//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-02
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_HASH_FIX_HXX
#define HOWARD11_HASH_FIX_HXX

#include <type_traits>

namespace Howard {

template<class E>
class enum_hash {
    using SFINAE_ = typename std::enable_if<std::is_enum<E>::value, E>::type;
public:
    size_t operator()(const E& e) const {
        return std::hash<typename std::underlying_type<E>::type>()(e); }
};

}

#endif // HOWARD11_HASH_FIX_HXX
