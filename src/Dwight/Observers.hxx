//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love. 2015-08-04.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_OBSERVERS_HXX
#define HOWARD11_OBSERVERS_HXX

#include "Common.hxx"
#include "HowardBase.hxx"
#include "Debug.hxx"

#include "Event.hxx"

#include <map>
#include <unordered_map>

namespace Howard {

class HNode;
class EventListenerBase;
class HEvent;
class ScriptEvent;

enum EventQueueType {
    QueueTypeGlobalEventBase,
    QueueTypeGlobalEventBaseMapped,
    QueueTypeUpdate,
    QueueTypePaint,
    QueueTypeEnd
};

class QueueGlobalEventBase : public HowardBase {
public:

    HowardRTTIType WhatAmI() const override { return HowardRTTIType::TEventQueueGlobal; }
    const char *class_name() const override { return QueueGlobalEventBase::m_class_name; }
    virtual const char *queue_name() const { return QueueGlobalEventBase::m_queue_name; }
    virtual EventQueueType queue_type() const { return EventQueueType::QueueTypeGlobalEventBase; }
    static constexpr const char m_class_name[] = "QueueGlobalEventBase";
    static constexpr const char m_queue_name[] = "GlobalEventBase";

    virtual ~QueueGlobalEventBase() { }

    // again, for the JS
    virtual bool inside(HNode *node) const { return false; };
    virtual bool add(HNode *node) { return false; };
    virtual bool remove(HNode *node) { return false; };

    void invoke() {
        this->will_invoke();
        this->_invoke();
        this->did_invoke();
    }

    virtual void will_invoke() { }
    virtual void did_invoke() { }

    virtual bool enabled(HNode *node) const { return false; };
    virtual bool disable(HNode *node) { return false; };
    virtual bool enable(HNode *node) { return false; };

protected:

    virtual void _invoke() { return; };
    virtual void invoke_per_node(HNode *node) { return; };
};

class QueueGlobalEventBaseMapped : public QueueGlobalEventBase {
public:

    HowardRTTIType WhatAmI() const override { return HowardRTTIType::TEventQueueGlobal; }
    const char *class_name() const override { return QueueGlobalEventBaseMapped::m_class_name; }
    const char *queue_name() const override { return QueueGlobalEventBaseMapped::m_queue_name; }
    EventQueueType queue_type() const override { return EventQueueType::QueueTypeGlobalEventBaseMapped; }
    static constexpr const char m_class_name[] = "QueueGlobalEventBaseMapped";
    static constexpr const char m_queue_name[] = "GlobalEventBaseMapped";

    bool inside(HNode *node) const override {
        auto i = m_nodes.find(node);
        return (i != m_nodes.end());
    }

    bool add(HNode *node) override {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i == m_nodes.end()) {
            m_nodes.insert({ node, true });
            return true;
        }
        return false;
    }

    bool remove(HNode *node) override {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i != m_nodes.end()) {
            m_nodes.erase(i);
            return true;
        }
        return false;
    }

    bool enabled(HNode *node) const override {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i != m_nodes.end()) {
            return i->second;
        }
        return false;
    }

    bool disable(HNode *node) override {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i != m_nodes.end()) {
            i->second = false;
            return true;
        }
        return false;
    }

    bool enable(HNode *node) override {
        ASSERT(node);
        auto i = m_nodes.find(node);
        if (i != m_nodes.end()) {
            i->second = true;
            return true;
        }
        return false;
    }

private:

    void _invoke() override {
        for (auto n : m_nodes) {
            if (n.second) {
                this->invoke_per_node(n.first);
            }
        }
    }

    std::map<HNode *, bool> m_nodes;
};

namespace Utility {

class InitializeOnlyOnce {
public:
    inline void initializeOnce() {
        ASSERT(!m_inited);
        m_inited = true;
    }

private:
    bool m_inited = false;
};

template <typename T>
class InitializeGlobalOnlyOnce {
public:
    inline void initializeOnce() {
        ASSERT(!inited);
        inited = true;
    }

private:
    static bool inited;
};

template <typename T>
bool InitializeGlobalOnlyOnce<T>::inited = false;

}

class QueueGlobalUpdate;
class QueueGlobalPaint;

class EventQueueManager {
public:

    void initialize();

    QueueGlobalEventBase *queue(EventQueueType type);
    QueueGlobalEventBase *queue(unsigned int qid);

    inline QueueGlobalUpdate *updateQueue() {
        return m_qupdate; }
    inline QueueGlobalPaint *paintQueue(){
        return m_qpaint; }

private:

    Utility::InitializeGlobalOnlyOnce<EventQueueManager> m_init;
    QueueGlobalUpdate *m_qupdate;
    QueueGlobalPaint *m_qpaint;

    std::unordered_map<unsigned int, QueueGlobalEventBase *> m_queues;
};

class EventNotificationGlobal : public HowardBase {
public:

private:

//    std::unordered_map<EventType, std::map<Node *, bool>> m_nodes;
//    std::unordered_map<EventTypeExt, std::map<Node *, bool>> m_script_nodes;
};

}

#endif // HOWARD11_OBSERVERS_HXX
