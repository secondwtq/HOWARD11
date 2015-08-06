
print("I'm preload.js!");

var root_node = getRootNode();
var asset_manager = AssetManager.prototype.instance;

print(root_node, ' ', root_node.node_type());
print(root_node.parent());
print(AssetManager.prototype.instance.named('node_tex'));

var new_node = StannumSpriteNode.prototype.create(root_node, asset_manager.named('node_tex'));
new_node.attach_to(root_node);
