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

#ifndef HOWARD11_SCRIPTNODE_HXX
#define HOWARD11_SCRIPTNODE_HXX

#include "Common.hxx"
#include "Node.hxx"

#include "../thirdpt/mozjs.hxx"
#include "ScriptEvent.hxx"

namespace Howard {

// be 'final', sure?
class ScriptNodeBase final : public HNode {

public:

    ScriptNodeBase (RootNode *scene, xoundation::spd::context_reference context) :
            HNode(scene), m_scriptctx(context) { printf("ScriptNode constructing ...\n"); }
    ScriptNodeBase (HandleObj<HNode> scene, xoundation::spd::context_reference context) :
            HNode(scene), m_scriptctx(context) { }

    static ScriptNodeBase *create(RootNode *scene, xoundation::spd::context_reference context) {
        return new ScriptNodeBase(scene, context); }

    HowardNodeType node_typeid() const override {
        return HowardNodeType::NScriptNode; }
    const char *node_type() const override {
        return ScriptNodeBase::m_node_type; }
    static constexpr const char m_node_type[] = "ScriptNode";

    void onUpdate() override;
    void onEvent(HEvent::shared_ptr_t event) override;
    void onScriptEvent(ScriptEventBase::shared_ptr_t event) override;

private:
    JSContext *m_scriptctx;
};

}

#endif // HOWARD11_SCRIPTNODE_HXX
