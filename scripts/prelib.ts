
var Global = Foundation;
var root = Foundation.rootNode;

class ScriptNode implements ScriptNodeInterface {

	constructor(scene: RootNode) { }

	WhatAmI: () => HowardRTTIType;
	class_name: () => string;

	RTTIID: number;
	node_typeid: () => HowardNodeType;
	node_type: () => string;

	parent: () => HNode;
	has_parent: () => boolean;
	has_child: (child: HNode) => boolean;
	add_child: (child: HNode) => void;
	detach_child: (child: HNode) => void;
	attach_to: (parent: HNode) => void;
	detach_from_parent: () => void;
	root: () => RootNode;
	
	length: number;
	child: (idx: number) => HNode;

	addListener: (type: EventType, typext: number, listener: EventListenerBase) => EventListenerBase;
	addScriptListener: (typext: number, listener: EventListenerBase) => EventListenerBase;
	removeListener: (type: EventType, listener: EventListenerBase) => boolean;
	invoke_event: (event: HEvent) => void;
	
	onUpdate() { }
	onEvent(event: HEvent) { }
	onScriptEvent(event: ScriptEventAny) { }
}
ScriptNode.prototype = ScriptNodeBase.prototype;
ScriptNode.prototype.constructor = ScriptNode;

function createScriptNode<T extends ScriptNode>(type: { new (RootNode): T, prototype: any }, ... args: any[]) : T {
	var ret = ScriptNodeBase.reproto(type.prototype);
	ScriptNodeBase.attachNew.call(ret, args[0]);
	type.apply(ret, args);
	return <T>ret;
}

function createScriptEvent<T>(typext: number, data: T) : ScriptEventBase<T> {
	var ret = ScriptEventBase.createShared<T>(typext);
	ret.data = data;
	return ret;
}
