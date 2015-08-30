
print("I'm preload.js!");

var unitimg: TextureImage = cast(Global.assetManager.named('TestUnit'), TextureImage);
var unittex: Texture = Texture.createWithPartialImage('TestUnit_aFrame',
	unitimg, new HPixel(835, 418), new HPixel(119, 93));
	
var dotimg = cast(Global.assetManager.named('dot'), TextureImage);
var dottex = Texture.createWithEntireImage('dot_image', dotimg);

class TestScriptNode extends ScriptNode {

	childNode: StannumSpriteNode;
	
	mouseDragging: boolean = false;
	lastPoint: HPoint = new HPoint(0, 0);

	constructor(scene: RootNode) {
		super(scene);
		Global.eventQueues.queueMouseMove.add(this);
		Global.eventQueues.queueMouseButton.add(this);
	}

	onUpdate() {
		cast(this.child(0).child(0), HammerActorNode).addForce(new HAnyCoord(0, 0, 100));
	}
	
	onEvent(event: HEvent) {
		if (event.event_type() == EventType.EInputEvent) {
			var eventData = cast(event, InputEvent).data;

			if (eventData.type == FacerEventType.MousePress) {
				this.mouseDragging = true;
			} else if (eventData.type == FacerEventType.MouseRelease) {
				this.mouseDragging = false;
			}
			if (eventData.type == FacerEventType.MouseMove) {
				
				if (this.mouseDragging) {
					var coord: HAnyCoord = this.childNode.position;
					var deltaX = eventData.cursorPositionX - this.lastPoint.x;
					var deltaY = eventData.cursorPositionY - this.lastPoint.y;
					coord.x += deltaX;
					coord.y += deltaY;
					this.childNode.set_position(coord);
				}
				
				this.lastPoint.x = eventData.cursorPositionX;
				this.lastPoint.y = eventData.cursorPositionY;
			}
		}
	}
	
	onScriptEvent(event: ScriptEventAny) {
		print(EventType[event.event_type()], ' ', event.data);
	}
}

var new_node: TestScriptNode = createScriptNode.call(TestScriptNode, root);
new_node.attach_to(root);
Global.eventQueues.queueUpdate.add(new_node);

var sprite_node = StannumSpriteNode.create(root, unittex);
sprite_node.attach_to(new_node);
sprite_node.set_position(new HAnyCoord(1024, 0, 0));
Global.eventQueues.queuePaint.add(sprite_node);
new_node.childNode = sprite_node;

function createNode(parent: HNode, 
	position: HAnyCoord) : StannumSpriteNode {
	var ret = StannumSpriteNode.create(root, dottex);
	ret.attach_to(parent);
	Global.eventQueues.queuePaint.add(ret);
	ret.set_position(position);
	return ret;
}

createNode(new_node, new HAnyCoord(0, 0, 0));
createNode(new_node, new HAnyCoord(8, 0, 0));
createNode(new_node, new HAnyCoord(8, 8, 0));
createNode(new_node, new HAnyCoord(0, 8, 0));

createNode(new_node, new HAnyCoord(128, 0, 0));
createNode(new_node, new HAnyCoord(256, 0, 0));

createNode(new_node, new HAnyCoord(64, 64, 0));
createNode(new_node, new HAnyCoord(128, 64, 0));
createNode(new_node, new HAnyCoord(192, 64, 0));
createNode(new_node, new HAnyCoord(256, 64, 0));

root.invoke_event(HEvent.createShared());
root.invoke_event(createScriptEvent<number>(0, 1));

var phys_node = new HammerActorNode(root, Transform.createPositioned(new HAnyCoord(192, 192, 96)));
var t = HQuaternion.normalize(HQuaternion.createEuler(new HAnyCoord(0, 0, 0.5235)));
print(t.x, ' ', t.y, ' ', t.z, ' ', t.w, ' ', HQuaternion.length(t));
var phys_body = new HammerPrimitiveBody(Foundation.hammerFoundation.defaultMaterial);
phys_body.addBox(Transform.createDefault(), new HAnyCoord(12, 12, 12));

HammerPrimitiveHelper.attachPrimitivesToActor(phys_body, phys_node);
phys_node.attach_to(sprite_node);
phys_node.addToScene(Foundation.mainPhysScene);

class ListenerDataPhysicsAttach
		extends EventListenerScriptData<ListenerDataPhysicsAttach> {

	onEvent(event: HEvent) {
		if (event.event_type() == EventType.EHammerTransformEvent) {
			var transform = cast(event, HammerTransformEvent).transform;
			var parent = cast(this.listener.listenerParent.parent(), StannumSpriteNode);
			parent.set_position(transform.position);
			// print(transform.position.z, ' ', cast(this.listener.listenerParent, HammerActorNode).mass);
		}
	}
}

var listener: EventListenerScript<ListenerDataPhysicsAttach> =
	createScriptListener.call(ListenerDataPhysicsAttach, "listener");
phys_node.addListener(EventType.EHammerTransformEvent, listener);
