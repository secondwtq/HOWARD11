
print("I'm preload.js!");

var node_image: TextureImage = <TextureImage>Global.assetManager.named('node');
var node_texture: Texture = Texture.createWithEntireImage('node_all', node_image);

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
					var coord: HCoord = this.childNode.position;
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

var sprite_node = StannumSpriteNode.create(root, node_texture);

var new_node: TestScriptNode = createScriptNode(TestScriptNode, root);
new_node.attach_to(root);
Global.eventQueues.queueUpdate.add(new_node);

sprite_node.attach_to(new_node);
sprite_node.set_position(new HCoord(1024, 0, 0));
Global.eventQueues.queuePaint.add(sprite_node);
new_node.childNode = sprite_node;

root.invoke_event(HEvent.createShared());
root.invoke_event(createScriptEvent<number>(0, 1));
