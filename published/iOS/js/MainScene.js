//
// MainScene class
//
var MainScene = function(){};

MainScene.prototype.onDidLoadFromCCB = function () {
};

// Create callback for button
MainScene.prototype.onPressButton = function()
{
  var scene = cc.BuilderReader.loadAsScene("interface/GameScene.ccbi");
  cc.Director.getInstance().replaceScene(scene);
};
