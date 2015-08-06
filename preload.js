
print("I'm preload.js!");

var root_node = getRootNode();
var asset_manager = AssetManager.prototype.instance;

print(root_node, ' ', root_node.node_type());
print(root_node.parent());
print(AssetManager.prototype.instance.named('node_tex'));

var node_image = asset_manager.named('node');
var node_texture = Texture.prototype.createWithEntireImage('node_all', node_image);
var new_node = StannumSpriteNode.prototype.create(root_node, node_texture);
new_node.attach_to(root_node);
new_node.set_position(new HCoord(1024, 0, 0));
