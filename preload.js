/// <reference path="HOWARD.d.ts" />
var __extends = this.__extends || function (d, b) {
    for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p];
    function __() { this.constructor = d; }
    __.prototype = b.prototype;
    d.prototype = new __();
};
print("I'm preload.js!");
var Global = Foundation;
// www.bennadel.com/blog/2291-invoking-a-native-javascript-constructor-using-call-or-apply.htm
var ScriptNode = (function () {
    function ScriptNode(scene) {
    }
    return ScriptNode;
})();
ScriptNode.prototype = ScriptNodeBase.prototype;
ScriptNode.prototype.constructor = ScriptNode;
var TestScriptNode = (function (_super) {
    __extends(TestScriptNode, _super);
    function TestScriptNode() {
        _super.apply(this, arguments);
        this.test = 3;
    }
    TestScriptNode.prototype.onUpdate = function () {
        print("I'm updating!", ' ', this.test);
    };
    return TestScriptNode;
})(ScriptNode);
function createScriptNode(type) {
    var args = [];
    for (var _i = 1; _i < arguments.length; _i++) {
        args[_i - 1] = arguments[_i];
    }
    var ret = ScriptNodeBase.reproto(type.prototype);
    ScriptNodeBase.attachNew.call(ret, args[0]);
    type.apply(ret, args);
    return ret;
}
var node_image = Global.assetManager.named('node');
var node_texture = Texture.createWithEntireImage('node_all', node_image);
var sprite_node = StannumSpriteNode.create(Global.rootNode, node_texture);
sprite_node.attach_to(Global.rootNode);
sprite_node.set_position(new HCoord(1024, 0, 0));
Global.eventQueues.paintQueue.add(sprite_node);
var new_node = createScriptNode(TestScriptNode, Global.rootNode);
print(new_node);
new_node.attach_to(Global.rootNode);
Global.eventQueues.updateQueue.add(new_node);
