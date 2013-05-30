/*
 Boot file for cocos2d-x + JS bindings
 */

// JS Bindings constants
require("jsb.js");

// Add here other files that are needed for your game
// Shared between cocos2d-html5 and cocos2d-x + JSB
// eg:
// require("levels.js");

// main game file. Shared between cocos2d-html5 and cocos2d-x + JSB
require('main.js');


// run code:
function run() {

    // Tell CocosBuilderReader to load
    cc.BuilderReader.setResourcePath("res/CCB/");

    var scene = cc.Scene.create();

    // main menu
    var menu = new MainLayer();
    scene.addChild(menu);

    var runningScene = director.getRunningScene();
    if (runningScene === null)
        director.runWithScene(scene);
    else
        director.replaceScene(cc.TransitionFade.create(0.5, scene));
}

run();
