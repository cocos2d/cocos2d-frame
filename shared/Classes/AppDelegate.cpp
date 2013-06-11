#include "AppDelegate.h"

static bool firstTime = true;

USING_NS_CC;
using namespace CocosDenshion;

extern "C" {
    const char * getCCBDirectoryPath();


    static void setResolutionSizes(bool isTablet, bool isRetina) {
        CCConfiguration *conf = CCConfiguration::sharedConfiguration();
        
        bool isPortrait = conf->getBool("cocos2d.orientation.isPortrait", true);
        double tabletW = conf->getNumber("cocos2d.designSize.tabletWidth", 0);
        double tabletH = conf->getNumber("cocos2d.designSize.tabletHeight", 0);
        double phoneW = conf->getNumber("cocos2d.designSize.phoneWidth", 0);
        double phoneH = conf->getNumber("cocos2d.designSize.phoneHeight", 0);
        
        if(tabletH == 0 || tabletW == 0) {
            if(isPortrait) {
                tabletH = 1024; tabletW = 768;
            } else {
                tabletH = 768; tabletW = 1024;
            }
        }
        
        if(phoneH == 0 || phoneW == 0) {
            if(isPortrait) {
                phoneH = 480; phoneW = 320;
            } else {
                phoneH = 320; phoneW = 480;
            }
        }
        
        CCSize designSize, resourceSize;

        // Is tablet or not
        designSize = CCSizeMake((isTablet) ? tabletW : phoneW, (isTablet) ? tabletH : phoneH);
        
        // Is retina -> scale by 2
        CCDirector::sharedDirector()->setContentScaleFactor((isRetina) ? 2.0 : 1.0);
        
        // Is portrait -> kResolutionFixedWidth, !portait -> kResolutionFixedHeight
        CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width,
                                                               designSize.height,
                                                               isPortrait ? kResolutionFixedWidth : kResolutionFixedHeight);
        
    }

	void handle_signal(int signal)
	{
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
}

//
// AppDelegate
//

AppDelegate::AppDelegate(): isRetina(false), isIPhone(false), _resolution("")
{
}

AppDelegate::~AppDelegate()
{
    CCScriptEngineManager::sharedManager()->purgeSharedManager();
}

void AppDelegate::setIsPortrait(bool isPortrait)
{
    CCBool *orient = CCBool::create(isPortrait);
    CCConfiguration *conf = CCConfiguration::sharedConfiguration();
    conf->setObject("cocos2d.orientation.isPortrait", orient);
}

static const char* getCCBWriteablePath() {
    std::string writeablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
    writeablePath += "/ccb/";
    return writeablePath.c_str();
}

void AppDelegate::initPlayer()
{
#if CC_PLAYER
	// CocosPlayer Resources
	std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
	searchPaths.insert(searchPaths.begin(), "PlayerResources");
    searchPaths.insert(searchPaths.begin(), getCCBWriteablePath());
    searchPaths.insert(searchPaths.begin(), CCFileUtils::sharedFileUtils()->getWritablePath());
	CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

	setDeviceResolution(_resolution.c_str());
	createPlayerServer(0);

	if(firstTime) {
		runMainScene();
		firstTime = false;
	} else {
		handle_ccb_run();
	}
#endif // CC_PLAYER
}

void AppDelegate::initJavaScript()
{
	CCConfiguration *conf = CCConfiguration::sharedConfiguration();

	// set search paths for JS
	std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();

	// for built-in JS files
	searchPaths.insert(searchPaths.begin(), "js");

	// User defined JS files
	searchPaths.insert(searchPaths.begin(), conf->getCString("cocos2d.jsb.search_path", "") );

	CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

	// get the name of the boot file
	const char *boot_jsb = conf->getCString("cocos2d.jsb.boot_file");

	// Register callbacks
	ScriptingCore* sc = ScriptingCore::getInstance();
	sc->addRegisterCallback(register_all_cocos2dx);
	sc->addRegisterCallback(register_all_cocos2dx_extension);
	sc->addRegisterCallback(register_cocos2dx_js_extensions);
	sc->addRegisterCallback(register_all_cocos2dx_extension_manual);
	sc->addRegisterCallback(register_CCBuilderReader);
	sc->addRegisterCallback(jsb_register_system);
	sc->addRegisterCallback(jsb_register_chipmunk);
	sc->addRegisterCallback(JSB_register_opengl);

#ifdef CC_PLAYER
	// CocosPlayer support could be added at compile time, but it can be disabled at runtime
	bool player_enabled = CCConfiguration::sharedConfiguration()->getBool("cocos2d.player.enabled");
	if( player_enabled )
		sc->addRegisterCallback(register_CocosPlayer);
#endif

	sc->start();

	CCScriptEngineProtocol *pEngine = ScriptingCore::getInstance();
	CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);

#ifdef CC_PLAYER
	if( player_enabled ) {
		initPlayer();
	} else
#endif // CC_PLAYER
    sc->runScript(boot_jsb);
}

void AppDelegate::initDirector()
{
	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	pDirector->setProjection(kCCDirectorProjection2D);

	// turn on display FPS ? If not found, set it as false
	bool display_fps = CCConfiguration::sharedConfiguration()->getBool("cocos2d.x.display_fps", false);
	pDirector->setDisplayStats(display_fps);

	// set FPS. If not found, use 60
	int fps = CCConfiguration::sharedConfiguration()->getNumber("cocos2d.x.fps", 60);
	pDirector->setAnimationInterval(1.0 / fps);
}

void AppDelegate::initSearchPath()
{
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    CCConfiguration *conf = CCConfiguration::sharedConfiguration();

	if(!conf->getBool("cocos2d.orientation.isPortrait", true)) {
		screenSize = CCSizeMake(screenSize.height, screenSize.width);
	}

	std::vector<std::string> resDirOrders;
	TargetPlatform platform = CCApplication::sharedApplication()->getTargetPlatform();


	if (platform == kTargetIphone || platform == kTargetIpad)
	{
		// CocosBuilderReader search path
		std::vector<std::string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
		searchPaths.insert(searchPaths.begin(), "Published files iOS");
		searchPaths.insert(searchPaths.begin(), getCCBDirectoryPath());
		CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

		if (screenSize.height > 1136)
		{
			_resolution = "iPad";
			setResolutionSizes(true, true);
			resDirOrders.push_back("resources-ipadhd");
			resDirOrders.push_back("resources-ipad");
			resDirOrders.push_back("resources-iphonehd");
			isIPhone = false;
			isRetina = true;
            
            // CCBReader::setResolutionScale(2) * sharedDirector()->setContentScaleFactor(1) = 2
            // == (iPad HD)
            // CocosBuilder scaling = 1,2,2,4 (iPhone, iPhoneRetina, iPad, iPadHD)
			cocos2d::extension::CCBReader::setResolutionScale(2);
            
		} else if(screenSize.height > 1024) {
			_resolution = "iPhone";
			setResolutionSizes(false, true);
			resDirOrders.push_back("resources-iphonehd");
			resDirOrders.push_back("resources-iphone");
			isIPhone = true;
			isRetina = true;
		}
		else if (screenSize.height > 960)
		{
			_resolution = "iPad";
			setResolutionSizes(true, false);
			resDirOrders.push_back("resources-ipad");
			resDirOrders.push_back("resources-iphonehd");
			isIPhone = false;
			isRetina = false;
            
            // CCBReader::setResolutionScale(2) * sharedDirector()->setContentScaleFactor(1) = 2
            // == (iPad)
            // CocosBuilder scaling = 1,2,2,4 (iPhone, iPhoneRetina, iPad, iPadHD)
			cocos2d::extension::CCBReader::setResolutionScale(2);

		}
		else if (screenSize.height > 480)
		{
			_resolution = "iPhone";
			setResolutionSizes(false, true);
			resDirOrders.push_back("resources-iphonehd");
			resDirOrders.push_back("resources-iphone");
			isIPhone = true;
			isRetina = true;
		}
		else
		{
			_resolution = "iPhone";
			setResolutionSizes(false, false);
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
				_resolution = "xlarge";
				setResolutionSizes(true, true);
				resDirOrders.push_back("resources-xlarge");
				resDirOrders.push_back("resources-large");
				resDirOrders.push_back("resources-medium");
				resDirOrders.push_back("resources-small");
			} else {
				_resolution = "large";
				setResolutionSizes(false, true);
				resDirOrders.push_back("resources-large");
				resDirOrders.push_back("resources-medium");
				resDirOrders.push_back("resources-small");
			}
		} else { // non retina
			if (screenSize.height > 960)
			{
				_resolution = "large";
				setResolutionSizes(true, false);
				resDirOrders.push_back("resources-large");
				resDirOrders.push_back("resources-medium");
				resDirOrders.push_back("resources-small");
                
                // CCBReader::setResolutionScale(2) * sharedDirector()->setContentScaleFactor(2) = 4
                // == (iPad retina)
				cocos2d::extension::CCBReader::setResolutionScale(2);
			}
			else if (screenSize.height > 768)
			{
				_resolution = "medium";
				setResolutionSizes(true, false);
				resDirOrders.push_back("resources-medium");
				resDirOrders.push_back("resources-small");
			}
			else if (screenSize.height > 480)
			{
				_resolution = "small";
				setResolutionSizes(false, false);
				resDirOrders.push_back("resources-small");
			}
			else
			{
				setResolutionSizes(false, false);
				_resolution = "xsmall";
				resDirOrders.push_back("resources-xsmall");
			}

		}
	}

	CCFileUtils *pFileUtils = CCFileUtils::sharedFileUtils();
	pFileUtils->setSearchResolutionsOrder(resDirOrders);

	std::vector<std::string> searchPaths = pFileUtils->getSearchPaths();
	searchPaths.insert(searchPaths.begin(), pFileUtils->getWritablePath());
	pFileUtils->setSearchPaths(searchPaths);

}

void AppDelegate::init()
{
	this->initDirector();
	this->initSearchPath();
}

bool AppDelegate::applicationDidFinishLaunching()
{
	//
	// 1:
	// Load config file
	//
	CCConfiguration *conf = CCConfiguration::sharedConfiguration();
	conf->loadConfigFile("config.plist");

	//
	// 2:
	// Initialize the OpenGL View
	// ( valid for JS and C++ games )
	//
	this->init();

	//
	// 3:
	// Initialize JavaScript
	// ( valid only for JS games )
	//
	this->initJavaScript();

	//
	// 4:
	// Add your native code here
	// ( valid only for C++ or C++ with JS games )
	//

	// --> ADD Your C++ code here <--

	return true;
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

