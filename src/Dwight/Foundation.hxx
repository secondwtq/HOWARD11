//
// Made by secondwtq <lovejay-lovemusic@outlook.com> with Love.
//
// Date: 2015-08-17
// Copyright (c) 2015 SCU ISDC All rights reserved.
//
// This file is part of the HOWARD11 Game Engine.
//
// WE ARE STANDING ON THE EDGE.
//

#ifndef HOWARD11_FOUNDATION_HXX
#define HOWARD11_FOUNDATION_HXX

class JSObject;
namespace JS {
template <typename T>
class PersistentRooted;
}

namespace xoundation {
class SpdRuntime;
}

namespace Howard {
class RootNode;
class EventQueueManager;
class AssetManager;
}

namespace Howard {

class FoundationInstance {
public:

    inline xoundation::SpdRuntime& JSRuntime() {
        return *m_JSRuntime; }
    inline JS::PersistentRooted<JSObject *>& JSGlobal() {
        return *m_JSGlobal; }

    inline RootNode& rootNode() {
        return *m_rootNode; }
    inline EventQueueManager& eventQueues() {
        return *m_eventQueues; }
    inline AssetManager& assetManager() {
        return *m_assetManager; }

    FoundationInstance& setJSRuntime(xoundation::SpdRuntime *runtime) {
        m_JSRuntime = runtime; return *this; }
    FoundationInstance& setJSGlobal(JS::PersistentRooted<JSObject *> *global) {
        m_JSGlobal = global; return *this; }

    FoundationInstance& setRootNode(RootNode *node) {
        m_rootNode = node; return *this; }
    FoundationInstance& setEventQueues(EventQueueManager *queues) {
        m_eventQueues = queues; return *this; }
    FoundationInstance& setAssetManager(AssetManager *assetmanager) {
        m_assetManager = assetmanager; return *this; }

private:

    xoundation::SpdRuntime *m_JSRuntime = nullptr;
    JS::PersistentRooted<JSObject *> *m_JSGlobal = nullptr;
    RootNode *m_rootNode = nullptr;
    EventQueueManager *m_eventQueues = nullptr;
    AssetManager *m_assetManager = nullptr;
};

extern FoundationInstance Foundation;

}

#endif // HOWARD11_FOUNDATION_HXX
