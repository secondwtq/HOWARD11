//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love. 2015-08-04.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#include "Common.hxx"
#include "Event.hxx"

#include "Observers.hxx"
#include "ObserversDef.hxx"

#include <memory>

namespace Howard {

constexpr const char QueueGlobalEventBase::m_class_name[];
constexpr const char QueueGlobalEventBase::m_queue_name[];
constexpr const char QueueGlobalEventBaseMapped::m_class_name[];
constexpr const char QueueGlobalEventBaseMapped::m_queue_name[];

constexpr const char QueueGlobalUpdate::m_class_name[];
constexpr const char QueueGlobalUpdate::m_queue_name[];
constexpr const char QueueGlobalPaint::m_class_name[];
constexpr const char QueueGlobalPaint::m_queue_name[];

constexpr const char QueueGlobalKeyboard::m_class_name[];
constexpr const char QueueGlobalKeyboard::m_queue_name[];
constexpr const char QueueGlobalMouseButton::m_class_name[];
constexpr const char QueueGlobalMouseButton::m_queue_name[];
constexpr const char QueueGlobalMouseMove::m_class_name[];
constexpr const char QueueGlobalMouseMove::m_queue_name[];

void QueueGlobalEventBaseMapped::dispatch_event(HEvent::shared_ptr_t event) {
    for (std::pair<HNode *, bool> i : m_nodes) {
        if (i.second) {
            i.first->invoke_event(event); }
    }
}

void EventQueueManager::initialize() {
    m_init.initializeOnce();

    this->m_qupdate = new QueueGlobalUpdate();
    this->m_qpaint = new QueueGlobalPaint();

    this->m_qkeyboard = new QueueGlobalKeyboard();
    this->m_qmousebtn = new QueueGlobalMouseButton();
    this->m_qmousemve = new QueueGlobalMouseMove();

    this->m_queues[EventQueueType::QueueTypeUpdate] = this->m_qupdate;
    this->m_queues[EventQueueType::QueueTypePaint] = this->m_qpaint;

    this->m_queues[EventQueueType::QueueTypeKeyboard] = this->m_qkeyboard;
    this->m_queues[EventQueueType::QueueTypeMouseButton] = this->m_qmousebtn;
    this->m_queues[EventQueueType::QueueTypeMouseMove] = this->m_qmousemve;
}

QueueGlobalEventBase *EventQueueManager::queue(unsigned int qid) {
    switch (qid) {
        case EventQueueType::QueueTypeUpdate:
            return m_qupdate;
        case EventQueueType::QueueTypePaint:
            return m_qpaint;
        default:
        {
            auto i = m_queues.find(qid);
            if (i != m_queues.end()) {
                return i->second; }
        }
            return nullptr;
    }
}

QueueGlobalEventBase *EventQueueManager::queue(EventQueueType type) {
    return queue(static_cast<unsigned int>(type)); }

}
