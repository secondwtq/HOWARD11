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

#ifndef HOWARD11_HANDLE_HXX
#define HOWARD11_HANDLE_HXX

#include "Common.hxx"
#include "HowardBase.hxx"

#include "HandleManager.hxx"

#include "Debug.hxx"

#include <utility>

namespace Howard {

template<typename T, typename ContainerT>
class Handle {

    public:

    Handle() : m_ref(-1) { }

    Handle(int id) : m_ref(id) { }
    Handle(const T *object) : Handle(object == nullptr ? -1 : object->RTTIID) { }

    HowardRTTIType WhatAmI() const { return HowardRTTIType::THandle; }

    const char *class_name() const { return "Handle"; }

    static constexpr const char *handle_type() { return T::m_class_name; }

    inline int id() const { return this->m_ref; }

    T *get() { return ContainerT::instance->get(m_ref); }

    const T *get() const { return ContainerT::instance->get(m_ref); }

//    template <typename ReturnT>
//    ReturnT *get() { return reinterpret_cast<ReturnT *>(ContainerT::instance->get(m_ref)); }
//
//    template <typename ReturnT>
//    const ReturnT *get() { return reinterpret_cast<ReturnT *>(ContainerT::instance->get(m_ref)); }

    Handle& operator = (void *ptr) {
        if (ptr == nullptr) {
            this->m_ref = -1; }
        else { ASSERT_FOUNDATION(); }
        return *this;
    }

    Handle& operator = (Handle<T, ContainerT> rhs) {
        swap(this, &rhs);
        return *this;
    }

    // stackoverflow.com/questions/4421706/operator-overloading
    // stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
    Handle& operator = (T *ptr) {
        if (ptr == nullptr) {
            this->m_ref = -1;
        } else {
            this->m_ref = ptr->RTTIID;
        }
        return *this;
    }

    operator bool() const {
        return this->m_ref != -1; }

    operator T *() const {
        return this->get(); }

    T *operator -> () {
        return this->get(); }

    const T *operator -> () const {
        return this->get(); }

    friend void swap(Handle *lhs, Handle *rhs) {
        using std::swap;

        swap(lhs->m_ref, rhs->m_ref);
    }

    private:

    int m_ref = -1;

};

template <typename T, typename ContainerT>
inline bool operator == (const Handle<T, ContainerT>& lhs, const Handle<T, ContainerT>& rhs) {
    return lhs.id() == rhs.id(); }

// originally 'HandleE.hxx'
class NodeManager;

template <typename T>
using HandleObj = Handle<T, NodeManager>;

}


#endif // HOWARD11_HANDLE_HXX
