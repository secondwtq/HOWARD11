/// <reference path="FacerCore.d.ts" />

declare enum HowardRTTIType {
	TBase,
	TNode,
	THandle,
	TAsset,
	TEventQueueGlobal,
	TEventNotificationCenter
}

declare enum HowardNodeType {
	NFoundation,
	NRootNode,
	NScriptNode,
	NStannumSpriteNode
}

declare enum EventType {
	ENone,
	EFoundation,
	EScriptEvent,
	EEnd
}

declare enum EventQueueType {
	QueueTypeGlobalEventBase,
	QueueTypeGlobalEventBaseMapped,
	QueueTypeUpdate,
	QueueTypePaint,
	QueueTypeEnd
}

declare function print(... args: any[]) : void;

declare class HCoord {

	constructor(x: number, y: number, z: number);

	x: number;
	y: number;
	z: number;
}

declare class HPoint {

	constructor(x: number, y: number, z: number);

	x: number;
	y: number;
}

interface HowardBase {
	WhatAmI() : HowardRTTIType;
	class_name() : string;
}

declare class Asset implements HowardBase {
	WhatAmI(): HowardRTTIType;
	class_name(): string;

	name() : string;
}

declare class AssetManager {
	named(name : string) : Asset;
}

declare class TextureImage extends Asset {
	
}

declare class Texture extends Asset {
	static createWithEntireImage(name: string, parent: TextureImage): Texture;
}

declare class HEvent {
	static createShared(): HEvent;

	event_type(): EventType;
	event_type_ext(): number;

	root(): HNode;

	stop_propagation(): void;
	stopped(): boolean;
}

declare class InputEvent extends HEvent {
	data: FacerEvent;
}

interface EventListenerBase extends HowardBase {
	
}

interface EventListenerScriptBase extends EventListenerBase {
	
}

declare class EventListenerScriptBaseData {
	listener : EventListenerScriptBase;
}

interface HNode extends EventListenerBase {
	RTTIID: number;
	node_typeid(): HowardNodeType;
	node_type(): string;

	parent(): HNode;
	has_parent(): boolean;
	has_child(child: HNode): boolean;
	add_child(child: HNode): void;
	detach_child(child: HNode): void;
	attach_to(parent: HNode): void;
	detach_from_parent(): void;
	root(): RootNode;

	addListener(type: EventType, typext: number, listener: EventListenerBase): EventListenerBase;
	addScriptListener(typext: number, listener: EventListenerBase): EventListenerBase;
	removeListener(type: EventType, listener: EventListenerBase): boolean;
	invoke_event(event: HEvent): void;
}

declare class HNodeImpl implements EventListenerBase {
	WhatAmI(): HowardRTTIType;
	class_name(): string;

	RTTIID: number;
	node_typeid(): HowardNodeType;
	node_type(): string;

	parent(): HNode;
	has_parent(): boolean;
	has_child(child: HNode): boolean;
	add_child(child: HNode): void;
	detach_child(child: HNode): void;
	attach_to(parent: HNode): void;
	detach_from_parent(): void;
	root(): RootNode;

	addListener(type: EventType, typext: number, listener: EventListenerBase): EventListenerBase;
	addScriptListener(typext: number, listener: EventListenerBase): EventListenerBase;
	removeListener(type: EventType, listener: EventListenerBase): boolean;
	invoke_event(event: HEvent): void;
}

declare class RootNode extends HNodeImpl { }

interface ScriptNodeInterface extends HNode { }

declare class ScriptNodeBase extends HNodeImpl implements ScriptNodeInterface {
	static create(scene: RootNode): ScriptNodeInterface;
	static attachNew(scene: RootNode): ScriptNodeInterface;
	static reproto(proto: any): ScriptNodeInterface;
}

declare class QueueGlobalEventBase implements HowardBase {
	WhatAmI(): HowardRTTIType;
	class_name(): string;

	queue_type(): EventQueueType;
	queue_name(): string;

	inside(node: HNode): boolean;
	add(node: HNode): boolean;
	remove(node: HNode): boolean;
}

declare class QueueGlobalUpdate extends QueueGlobalEventBase { }
declare class QueueGlobalPaint extends QueueGlobalEventBase { }

declare class EventQueueManager {
	updateQueue: QueueGlobalUpdate;
	paintQueue: QueueGlobalPaint;
	queue(qid: number): QueueGlobalEventBase;
}

declare class FoundationInstance {
	rootNode: RootNode;
	eventQueues: EventQueueManager;
	assetManager: AssetManager;
}

declare var Foundation: FoundationInstance;

declare class StannumSpriteNode extends HNodeImpl {
	static create(scene: RootNode, texture: Texture): StannumSpriteNode;
	// Set the sprite's position(3D, HCoord) in the world space
	set_position(pos: HCoord): void;
}
