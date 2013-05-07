#include "AppDelegate.h"

static bool firstTime = true;
static bool isPortraitApp = true;

USING_NS_CC;
using namespace CocosDenshion;



AppDelegate::AppDelegate(): isRetina(false), isIPhone(false)
{
}

AppDelegate::~AppDelegate()
{
    CCScriptEngineManager::sharedManager()->purgeSharedManager();
}

void AppDelegate::setIsPortrait(bool isPortrait) {
    isPortraitApp = isPortrait;
}

extern "C" {
    
    
    const char * getCCBDirectoryPath();
    
    
    static void setResolutionSizes(bool isTablet, bool isRetina, bool isPortrait) {
        CCSize designSize, resourceSize;
        if(isTablet && isPortrait) {
            designSize = CCSizeMake(768, 1024);
            if(isRetina) resourceSize = CCSizeMake(1536, 2048);
            else resourceSize = CCSizeMake(768, 1024);
        } else if(isTablet) {
            designSize = CCSizeMake(1024, 768);
            if(isRetina) resourceSize = CCSizeMake(2048, 1536);
            else resourceSize = CCSizeMake(1024, 768);
        } else if(isPortrait) {
            designSize = CCSizeMake(320, 480);
            if(isRetina) resourceSize = CCSizeMake(640, 960);
            else resourceSize = CCSizeMake(320, 480);
        } else {
            designSize = CCSizeMake(480, 320);
            if(isRetina) resourceSize = CCSizeMake(960, 640);
            else resourceSize = CCSizeMake(480, 320);
        }
        
        CCDirector::sharedDirector()->setContentScaleFactor(resourceSize.width/designSize.width);
        if(isPortrait) {
            CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedWidth);
        } else {
            CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedHeight);
        }
    }
    
    void AppDelegate::initGameView() {
        
        // initialize director
        CCDirector *pDirector = CCDirector::sharedDirector();
        pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
        pDirector->setProjection(kCCDirectorProjection2D);
        
        
        CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
        if(!isPortraitApp) {
            screenSize = CCSizeMake(screenSize.height, screenSize.width);
        }
        
        std::vector<std::string> resDirOrders;
        std::string res;
        TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();
        
        
        if (platform == kTargetIphone || platform == kTargetIpad)
        {
            std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
            searchPaths.insert(searchPaths.begin(), "Published files iOS");
            searchPaths.insert(searchPaths.begin(), getCCBDirectoryPath());
            
            CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
            if (screenSize.height > 1136)
            {
                res = "iPad";
                setResolutionSizes(true, true, isPortraitApp);
                resDirOrders.push_back("resources-ipadhd");
                resDirOrders.push_back("resources-ipad");
                resDirOrders.push_back("resources-iphonehd");
                isIPhone = false;
                isRetina = true;
                cocos2d::extension::CCBReader::setResolutionScale(2);
            } else if(screenSize.height > 1024) {
                res = "iPhone";
                setResolutionSizes(false, true, isPortraitApp);
                resDirOrders.push_back("resources-iphonehd");
                resDirOrders.push_back("resources-iphone");
                isIPhone = true;
                isRetina = true;
            }
            else if (screenSize.height > 960)
            {
                res = "iPad";
                setResolutionSizes(true, false, isPortraitApp);
                resDirOrders.push_back("resources-ipad");
                resDirOrders.push_back("resources-iphonehd");
                isIPhone = false;
                isRetina = false;
                cocos2d::extension::CCBReader::setResolutionScale(2);
                
            }
            else if (screenSize.height > 480)
            {
                res = "iPhone";
                setResolutionSizes(false, true, isPortraitApp);
                resDirOrders.push_back("resources-iphonehd");
                resDirOrders.push_back("resources-iphone");
                isIPhone = true;
                isRetina = true;
            }
            else
            {
                res = "iPhone";
                setResolutionSizes(false, false, isPortraitApp);
                resDirOrders.push_back("resources-iphone");
                isIPhone = true;
                isRetina = false;
            }
            
        }
        else if (platform == kTargetAndroid || platform == kTargetWindows)
        {
            int dpi = -1;
            dpi = CCDevice::getDPI();
            
            if(dpi > 300) { // retina
                if (screenSize.height > 1920) {
                    res = "xlarge";
                    setResolutionSizes(true, true, isPortraitApp);
                    resDirOrders.push_back("resources-xlarge");
                    resDirOrders.push_back("resources-large");
                    resDirOrders.push_back("resources-medium");
                    resDirOrders.push_back("resources-small");
                } else {
                    res = "large";
                    setResolutionSizes(false, true, isPortraitApp);
                    resDirOrders.push_back("resources-large");
                    resDirOrders.push_back("resources-medium");
                    resDirOrders.push_back("resources-small");
                }
            } else { // non retina
                if (screenSize.height > 960)
                {
                    res = "large";
                    setResolutionSizes(true, false, isPortraitApp);
                    resDirOrders.push_back("resources-large");
                    resDirOrders.push_back("resources-medium");
                    resDirOrders.push_back("resources-small");
                    cocos2d::extension::CCBReader::setResolutionScale(2);
                }
                else if (screenSize.height > 768)
                {
                    res = "medium";
                    setResolutionSizes(true, false, isPortraitApp);
                    resDirOrders.push_back("resources-medium");
                    resDirOrders.push_back("resources-small");
                }
                else if (screenSize.height > 480)
                {
                    res = "small";
                    setResolutionSizes(false, false, isPortraitApp);
                    resDirOrders.push_back("resources-small");
                }
                else
                {
                    setResolutionSizes(false, false, isPortraitApp);
                    res = "xsmall";
                    resDirOrders.push_back("resources-xsmall");
                }
                
            }
        }
        
        CCFileUtils *pFileUtils = CCFileUtils::sharedFileUtils();
        pFileUtils->setSearchResolutionsOrder(resDirOrders);
        
        std::vector<std::string> searchPaths = pFileUtils->getSearchPaths();
        searchPaths.insert(searchPaths.begin(), pFileUtils->getWritablePath());
        pFileUtils->setSearchPaths(searchPaths);
        
        PlayerStatus::setDeviceResolution(res);
        // turn on display FPS
        pDirector->setDisplayStats(true);
        
        // set FPS. the default value is 1.0/60 if you don't call this
        pDirector->setAnimationInterval(1.0 / 60);
    }
    
    bool AppDelegate::applicationDidFinishLaunching()
    {
        
        initGameView();
        ScriptingCore* sc = ScriptingCore::getInstance();
        sc->addRegisterCallback(register_all_cocos2dx);
        sc->addRegisterCallback(register_all_cocos2dx_extension);
        sc->addRegisterCallback(register_cocos2dx_js_extensions);
        sc->addRegisterCallback(register_all_cocos2dx_extension_manual);
        sc->addRegisterCallback(register_CCBuilderReader);
        sc->addRegisterCallback(jsb_register_system);
        sc->addRegisterCallback(jsb_register_chipmunk);
        sc->addRegisterCallback(JSB_register_opengl);
        sc->addRegisterCallback(register_CocosPlayer);
        sc->start();
        
        CCScriptEngineProtocol *pEngine = ScriptingCore::getInstance();
        CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
        if(firstTime) {
            runMainScene();
            firstTime = false;
        } else {
            handle_ccb_run();
        }
    }
    
    
    
    void handle_signal(int signal) {
        static int internal_state = 0;
        ScriptingCore* sc = ScriptingCore::getInstance();
        // should start everything back
        CCDirector* director = CCDirector::sharedDirector();
        if (director->getRunningScene()) {
            director->popToRootScene();
        } else {
            CCPoolManager::sharedPoolManager()->finalize();
            if (internal_state == 0) {
                //sc->dumpRoot(NULL, 0, NULL);
                sc->start();
                internal_state = 1;
            } else {
                sc->runScript("hello.js");
                internal_state = 0;
            }
        }
    }
    
    // This function will be called when the app is inactive. When comes a phone call,it's be invoked too
    void AppDelegate::applicationDidEnterBackground()
    {
        CCDirector::sharedDirector()->stopAnimation();
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    }
    
    // this function will be called when the app is active again
    void AppDelegate::applicationWillEnterForeground()
    {
        CCDirector::sharedDirector()->startAnimation();
        SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
        SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    }
    
}
