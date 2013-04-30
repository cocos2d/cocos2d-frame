# cocos2d-player
cocos2d-player JS is an Android player for running Javascript based CocosBuilder projects. 

It is a wireless sync tool that works in conjunction with CocosBuilder. 

Using cocos2d-player, you can publish your CocosBuilder JS project and run them wirelessly, directly from within CocosBuilder. 

The system uses bonjour/zeroconf protocol for network discovery and connection. No configuration/rebuilding required when changes are made to the CocosBuilder project.

## Minimum Requirements

### Android

Currently cocos2d-player for Android only works with OS version >= 4.1. API level >= 16. This is because zeroconf has only been implemented in API level 16.

## Supported Devices

### iOS

iPhone (All versions), iPad (All versions), iPod

### Android

Currently cocos2d-player is supported on the following devices. NOTE: This is not an exaustive list. It only contains devices that cocos2d-player for Android has been tested on.

Google Nexus 4, Google Nexus 10, Google Nexus 7, Samsung Galaxy Nexus, Samsung Galaxy Note, Samsung Galaxy Note 2, Samsung Galaxy SIII.

## Build Instructions

```
$ git clone git@github.com:rohankuruvilla/cocos2d-player.git .
$ cd cocos2d-player/
$ git submodule update --init --recursive
```

### iOS

`$ open proj.ios/CocosPlayer.xcodeproject/`

### Android

* Follow README instructions in proj.android/


## How to use

1. Start the cocos2d-player application.  
2. On CocosBuilder, Publish and run project
3. Set the pairing code. By default it is set as Auto on cocos2d-player - corresponding to empty pairing code on CocosBuilder. Only numbers are supported as pairing codes.
4. It will show connected, and should automatically run the CocosBuilder JS project after a short delay.
5. Next time you close and open the app. The *run* and *reset* buttons will be enabled, allowing you to run the last cached CocosBuilder project and clean the last cached CocosBuilder project respectively.
