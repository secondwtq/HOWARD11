
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
	EInputEvent,
	EScriptEvent,
	EHammerTransformEvent,
	EEnd
}

declare enum EventQueueType {
	QueueTypeGlobalEventBase,
	QueueTypeGlobalEventBaseMapped,
	
	QueueTypeUpdate,
	QueueTypePaint,
	
	QueueTypeKeyboard,
	QueueTypeMouseButton,
	QueueTypeMouseMove,
	
	QueueTypeEnd
}

declare class HCoord {

	constructor(x: number, y: number, z: number);

	x: number;
	y: number;
	z: number;
}

declare class HPoint {

	constructor(x: number, y: number);

	x: number;
	y: number;
}

declare class HPixel {
	
	constructor(x: number, y: number);
	
	x: number;
	y: number;
}

declare class HAnyCoord {
	
	constructor(x: number, y: number, z: number);
	
	x: number;
	y: number;
	z: number;
}

declare class HQuaternion {
	
	constructor(x: number, y: number, z: number, w: number);
	
	x: number;
	y: number;
	z: number;
	w: number;
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
	static createWithPartialImage(name: string, parent: TextureImage,
		position: HPixel, size: HPixel): Texture;
}

declare class HEvent {
	static createShared(): HEvent;

	event_type(): EventType;
	event_type_ext(): number;

	root(): HNode;
	source: HNode;

	stop_propagation(): void;
	stopped(): boolean;
}

declare class InputEvent extends HEvent {
	data: FacerEvent;
}

declare class ScriptEventBase<T> extends HEvent {
	
	static createShared(): HEvent;
	static createShared<T>(typext: number): ScriptEventBase<T>;
	
	data: T;
}

declare type ScriptEventAny = ScriptEventBase<any>;

interface EventListenerBase extends HowardBase {
	priority: number;
	listenerName: string;
	listenerParent: HNode;
}

declare class EventListenerScript<T> 
		implements EventListenerBase {
			
	static createShared<T>(name: string): EventListenerScript<T>;
			
	WhatAmI() : HowardRTTIType;
	class_name() : string;
	
	priority: number;
	listenerName: string;
	listenerParent: HNode;
	
	data: T;
}

interface EventListenerScriptDataBaseI<T> {	
	listener: EventListenerScript<T>;
}

declare class EventListenerScriptDataBase<T> implements EventListenerScriptDataBaseI<T> {
	static reproto<T>(proto: any): T;
	static attachNew<T>(listener: T):
		EventListenerScriptDataBase<T>;
	
	constructor(listener: EventListenerScript<EventListenerScriptDataBase<T>>);
	listener: EventListenerScript<T>;
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

	length: number;
	child(idx: number): HNode;

	addListener(type: EventType, listener: EventListenerBase): EventListenerBase;
	addScriptListener(typext: number, listener: EventListenerBase): EventListenerBase;
	removeListener(type: EventType, listener: EventListenerBase): boolean;
	invoke_event(event: HEvent): void;
}

declare class HNodeImpl implements HNode {
	WhatAmI(): HowardRTTIType;
	class_name(): string;

	priority: number;
	listenerName: string;
	listenerParent: HNode;

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

	length: number;
	child(idx: number): HNode;

	addListener(type: EventType, listener: EventListenerBase): EventListenerBase;
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
	
	onUpdate();
	onEvent(event: HEvent);
	onScriptEvent(event: ScriptEventAny);
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
	queueUpdate: QueueGlobalUpdate;
	queuePaint: QueueGlobalPaint;
	
	queueKeyboard: QueueGlobalEventBase;
	queueMouseButton: QueueGlobalEventBase;
	queueMouseMove: QueueGlobalEventBase;
	
	queue(qid: number): QueueGlobalEventBase;
}

declare class FoundationInstance {
	rootNode: RootNode;
	eventQueues: EventQueueManager;
	assetManager: AssetManager;
	
	hammerFoundation: HammerFoundation;
	mainPhysScene: HammerScene;
}

declare var Foundation: FoundationInstance;

declare class StannumSpriteNode extends HNodeImpl {
	static create(scene: RootNode, texture: Texture): StannumSpriteNode;
	
	position: HAnyCoord;
	// Set the sprite's position(3D, HAnyCoord) in the world space
	set_position(pos: HAnyCoord): void;
}

declare class Transform {
	position: HAnyCoord;
	rotation: HQuaternion;
	
	static create(position: HAnyCoord, rotation: HQuaternion): Transform;
	static createDefault(): Transform;
	static createPositioned(position: HAnyCoord): Transform;
	static createRotated(rotation: HQuaternion): Transform;
}
