//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love. 2015-08-17.
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_OBSERVERSDEF_HXX
#define HOWARD11_OBSERVERSDEF_HXX

#include "Common.hxx"
#include "Observers.hxx"
#include "Node.hxx"

#include "../Stannum/Stannum.hxx"

#include <memory>

namespace Howard {

class QueueGlobalUpdate : public QueueGlobalEventBaseMapped {
public:

    const char *class_name() const override { return QueueGlobalUpdate::m_class_name; }
    const char *queue_name() const override { return QueueGlobalUpdate::m_queue_name; }
    EventQueueType queue_type() const override {
        return EventQueueType::QueueTypeUpdate; }
    static constexpr const char m_class_name[] = "QueueGlobalUpdate";
    static constexpr const char m_queue_name[] = "GlobalUpdate";

    void invoke_per_node(HNode *node) override {
        node->onUpdate();
    }
};

class QueueGlobalPaint : public QueueGlobalEventBaseMapped {
public:

    const char *class_name() const override { return QueueGlobalPaint::m_class_name; }
    const char *queue_name() const override { return QueueGlobalPaint::m_queue_name; }
    EventQueueType queue_type() const override {
        return EventQueueType::QueueTypePaint; }
    static constexpr const char m_class_name[] = "QueueGlobalPaint";
    static constexpr const char m_queue_name[] = "GlobalPaint";

    QueueGlobalPaint& setRenderQueue(std::shared_ptr<Stannum::RenderQueue> queue) {
        this->queue = queue;
        return *this;
    }

    void will_invoke() override {
        ASSERT(queue != nullptr); }
    void did_invoke() override {
        queue.reset(); }

    void invoke_per_node(HNode *node) override {
        node->onPaint(queue.get()); }

private:

    std::shared_ptr<Stannum::RenderQueue> queue;
};

}

#endif // HOWARD11_OBSERVERSDEF_HXX
