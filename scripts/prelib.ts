
var Global = Foundation;
var root = Foundation.rootNode;

class ScriptNode implements ScriptNodeInterface {

	constructor(scene: RootNode) { }

	WhatAmI: () => HowardRTTIType;
	class_name: () => string;
	
	priority: number;
	listenerName: string;
	listenerParent: HNode;

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

	addListener: (type: EventType, listener: EventListenerBase) => EventListenerBase;
	addScriptListener: (typext: number, listener: EventListenerBase) => EventListenerBase;
	removeListener: (type: EventType, listener: EventListenerBase) => boolean;
	invoke_event: (event: HEvent) => void;
	
	onUpdate() { }
	onEvent(event: HEvent) { }
	onScriptEvent(event: ScriptEventAny) { }
}
ScriptNode.prototype = ScriptNodeBase.prototype;
ScriptNode.prototype.constructor = ScriptNode;

class EventListenerScriptData<T> implements EventListenerScriptDataBaseI<T> {
	constructor(name: string) { }
	listener: EventListenerScript<T>;
}
EventListenerScriptData.prototype = EventListenerScriptDataBase.prototype;
EventListenerScriptData.prototype.constructor = EventListenerScriptData;

function createScriptNode<T extends ScriptNode>(... args: any[]) : T {
	var ret = ScriptNodeBase.reproto(this.prototype);
	ScriptNodeBase.attachNew.call(ret, arguments[0]);
	this.apply(ret, arguments);
	return <T>ret;
}

function createScriptEvent<T>(typext: number, data: T) : ScriptEventBase<T> {
	var ret = ScriptEventBase.createShared<T>(typext);
	ret.data = data;
	return ret;
}

function createScriptListener<T>(name: string, ... args: any[]) 
		: EventListenerScript<T> {
	var ret = EventListenerScript.createShared<T>(name);
	
	var data = EventListenerScriptDataBase.reproto<T>(this.prototype);
	EventListenerScriptDataBase.attachNew.call(data, ret);
	this.apply(data, arguments);
	
	ret.data = data;
	return ret;
}
