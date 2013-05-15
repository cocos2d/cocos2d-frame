# cocos2d-frame
cocos2d-frame is an empty game project suitable for C++ and / or JS games. 
It includes the following components:
 - cocos2d-x
 - Chipmunk2d
 - JS bindings for cocos2d-x and Chipmunk2D
 - JS remote debugger 
 - CocosPlayer
 
 Those components could be disabled at compile time when needed. For example, if you are developing a JS game, you need all of those components while you are developing the game. But once your game is ready to be submitted to the AppStore, the JS remote debugger and CocosPlayer should be disabled.
 

![cocos2d frame architecture](https://lh3.googleusercontent.com/-FXcqgFHMckI/UZQaydD1oxI/AAAAAAAAuoo/3xLatbBkhcg/s640/Slide1.jpg)


## Starting a new cocos2d-project

```
$ git clone git://github.com/cocos2d/cocos2d-frame.git
$ cd cocos2d-frame
$ git submodule update --init --recursive
```

## cocos2d-console

cocos2d-frame is part of the cocos2d-console project. Basically if you have cocos2d-console installed and type:
```
$ cocos2d new "My Project"
``` 
then it will clone cocos2d-frame.
