//
//  Cocos2dPlayerCommon.cpp
//  CocosPlayer
//
//  Created by Rohan Kuruvilla on 01/05/2013.
//
//

#include "Cocos2dPlayerCommon.h"
#include "AppDelegate.h"

USING_NS_CC;
using namespace CocosDenshion;

CCScene *mainScene;

bool runMainScene() {
    /* Push the new scene with a fancy transition. */
    ccColor3B transitionColor;
    transitionColor.r = 0;
    transitionColor.g = 0;
    transitionColor.b = 0;
    
    mainScene = PlayerStatus::loadMainScene("PlayerResources/StatusLayer.ccbi");
    
    if(CCDirector::sharedDirector()->getRunningScene() != NULL && CCDirector::sharedDirector()->getRunningScene() != mainScene) {
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, mainScene, transitionColor));
    } else if(CCDirector::sharedDirector()->getRunningScene() != NULL) {
        
    }else {
        CCDirector::sharedDirector()->runWithScene(CCTransitionFade::create(0.5f, mainScene, transitionColor));
    }
    return true;
}

void openEditBox();
void updatePairingLabel(const char * label) {
    if(!label) {
        PlayerStatus::pairingLabel = "";
    } else {
        PlayerStatus::pairingLabel = label;
    }
}


void handle_set_message(const char* msg) {
    CCBHelper::setInstructionsMessage(msg);
}

void handle_set_status(const char* msg) {
    CCBHelper::setStatusMessage(msg);
}

void handle_disconnected() {
    CCBHelper::setStatusMessage("Disconnected");
}

void handle_ccb_stop() {
    SimpleAudioEngine::sharedEngine()->end();
    runMainScene();
}

void handle_ccb_run() {
    CCFileUtils::sharedFileUtils()->purgeCachedEntries();
    SimpleAudioEngine::sharedEngine()->end();
    CCFileUtils::sharedFileUtils()->loadFilenameLookupDictionaryFromFile("fileLookup.plist");
    ScriptingCore::getInstance()->runScript("main.js");
}

bool handle_eval_script(const char *script, jsval *out) {
    ScriptingCore::getInstance()->evalString(script, out);
    return true;
}


void handle_connected() {
    CCBHelper::setStatusMessage("Connected!");
}

static void setViewValues(CCEGLView *pView, CCSize frameSize, CCSize designSize) {
    CCFileUtils::sharedFileUtils()->purgeCachedEntries();
    ((AppDelegate *)CCApplication::sharedApplication())->init();
}

void handle_set_orient(bool isPortrait) {
    cocos2d::CCEGLView* pView = CCEGLView::sharedOpenGLView();
    if (pView != NULL)
    {
        CCSize frameSize = pView->getFrameSize();
        CCSize designSize = pView->getDesignResolutionSize();
        if (((frameSize.width > frameSize.height && isPortrait))
            || (frameSize.width < frameSize.height && !isPortrait)) {
            ((AppDelegate *)CCApplication::sharedApplication())->setIsPortrait(isPortrait);
            pView->setFrameSize(frameSize.height, frameSize.width);
            setViewValues(pView, frameSize, designSize);
        }
    }
}

