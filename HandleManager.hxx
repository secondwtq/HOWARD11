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
#include <unordered_set>

#include "Debug.hxx"

namespace Howard {

template<typename T>
class HandleManager {

    public:

    inline size_t count_obj() const { return m_ptrs.size(); }
    inline size_t count_site() const { return m_data.size(); }

    inline size_t allocate_site() {
        size_t ret = m_data.size();
        m_data.push_back(nullptr);
        return ret;
    }

    inline void init_site(std::size_t id, T *ptr) {
        if (this->m_data.size() > id && (get(id) == nullptr)) {
            m_data[id] = ptr;
        } else {
            ASSERT_FOUNDATION();
        }
    }

    inline void clear_site(std::size_t id) {
        if (this->m_data.size() > id) {
            m_data[id] = nullptr; }
    }

    inline T *get(int id) const {
        return (this->m_data.size() <= id) ? nullptr : this->m_data[id]; }

    inline bool object_is_null(int id) const {
        return ((this->m_data.size() <= id) || this->m_data[id] == nullptr); }

    private:

    std::vector<T *> m_data;
    std::unordered_set<T *> m_ptrs;
};

}

#endif // HOWARD11_HANDLEMANAGER_HXX
