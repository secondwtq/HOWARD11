
print("I'm preload.js!");

var node_image: TextureImage = <TextureImage>Global.assetManager.named('node');
var node_texture: Texture = Texture.createWithEntireImage('node_all', node_image);

class TestScriptNode extends ScriptNode {

	childNode: StannumSpriteNode;

	onUpdate() {
		var now: HCoord = this.childNode.position;
		now.z += 20;
		this.childNode.set_position(now);
	}
	
	onEvent(event: HEvent) {
		print("We received an event!");
		print(EventType[event.event_type()]);
	}
	
	onScriptEvent(event: ScriptEventAny) {
		print(EventType[event.event_type()], ' ', event.data);
	}
}

var sprite_node = StannumSpriteNode.create(root, node_texture);

var new_node: TestScriptNode = createScriptNode(TestScriptNode, root);
new_node.attach_to(root);
Global.eventQueues.updateQueue.add(new_node);

sprite_node.attach_to(new_node);
sprite_node.set_position(new HCoord(1024, 0, 0));
Global.eventQueues.paintQueue.add(sprite_node);
new_node.childNode = sprite_node;

root.invoke_event(HEvent.createShared());
root.invoke_event(createScriptEvent<number>(0, 1));
