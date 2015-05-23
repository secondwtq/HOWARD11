//
// Created by secondwtq <lovejay-lovemusic@outlook.com> 2015/05/23.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of ISDCNext.
//
// We have always treaded the borderland.
//

#ifndef HOWARD11_HANDLEMANAGER_HXX
#define HOWARD11_HANDLEMANAGER_HXX

#include <vector>

namespace Howard {

template<typename T>
class HandleManager {

    public:

    inline T *get(int id) {
        return (this->m_data.size() <= id) ? nullptr : this->m_data[id]; }

    inline bool object_is_null(int id) {
        return ((this->m_data.size() <= id) || this->m_data[id] == nullptr); }

    private:

    std::vector<T *> m_data;
};

}

#endif // HOWARD11_HANDLEMANAGER_HXX
