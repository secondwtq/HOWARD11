/// <reference path="HOWARD.d.ts" />

print("I'm preload.js!");
var Global = Foundation;

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

	addListener: (type: EventType, typext: number, listener: EventListenerBase) => EventListenerBase;
	addScriptListener: (typext: number, listener: EventListenerBase) => EventListenerBase;
	removeListener: (type: EventType, listener: EventListenerBase) => boolean;
	invoke_event: (event: HEvent) => void;
}
ScriptNode.prototype = ScriptNodeBase.prototype;
ScriptNode.prototype.constructor = ScriptNode;

class TestScriptNode extends ScriptNode {

	test: number = 3;

	onUpdate() {
		print("I'm updating!", ' ', this.test);
	}
}

function createScriptNode<T extends ScriptNode>(type: { new (RootNode): T, prototype: any }, ... args: any[]) : T {
	var ret = ScriptNodeBase.reproto(type.prototype);
	ScriptNodeBase.attachNew.call(ret, args[0]);
	type.apply(ret, args);
	return <T>ret;
}

var node_image: TextureImage = <TextureImage>Global.assetManager.named('node');
var node_texture: Texture = Texture.createWithEntireImage('node_all', node_image);
var sprite_node = StannumSpriteNode.create(Global.rootNode, node_texture);

sprite_node.attach_to(Global.rootNode);
sprite_node.set_position(new HCoord(1024, 0, 0));
Global.eventQueues.paintQueue.add(sprite_node);

var new_node: TestScriptNode = createScriptNode(TestScriptNode, Global.rootNode);
print(new_node);
new_node.attach_to(Global.rootNode);
Global.eventQueues.updateQueue.add(new_node);
