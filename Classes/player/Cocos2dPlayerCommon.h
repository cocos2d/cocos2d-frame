//
//  Cocos2dPlayerCommon.h
//  CocosPlayer
//
//  Created by Rohan Kuruvilla on 30/04/2013.
//
//

#ifndef CocosPlayer_Cocos2dPlayerCommon_h
#define CocosPlayer_Cocos2dPlayerCommon_h

#include "jsapi.h"
#include "PlayerStatus.h"
#include "MainSceneHelper.h"
#include "SimpleAudioEngine.h"
#include "ScriptingCore.h"


extern "C" {
    extern bool runMainScene();
    extern bool handle_eval_script(const char *script, jsval *out);
    extern void openEditBox();
    extern void updatePairingLabel(const char *);
    extern void handle_ccb_stop();
    extern void handle_connected();
    extern void handle_disconnected();
    extern void handle_set_message(const char *msg);
    extern void handle_set_status(const char *msg);
    extern void updatePairing(const char *code);
    extern void cleanCacheDir();
    extern void createPlayerServer(int port);
    extern void setDeviceResolution(const char* res);
    extern void sendLogMsg(const char* log);
    extern void resetCocosApp();
    extern void runJSApp();
    extern void handle_ccb_run();
    extern void handle_set_orient(bool isPortrait);
    extern void setOrientationJNI(int orient);
}

#endif
