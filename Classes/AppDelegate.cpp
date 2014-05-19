#include "AppDelegate.h"
#include "PlayStageScene.h"
#include "EntryPointScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
    
    srand(time(NULL));

    std::vector<std::string>searchPath;
    
    CCLOG("%f",glview->getFrameSize().width);
    if (glview->getFrameSize().width > 320.0f) {
        searchPath.push_back("hd");
    } else {
        searchPath.push_back("sd");
    }
    
    CCLOG("%s", searchPath.back().c_str());
    
    FileUtils::getInstance()->setSearchResolutionsOrder(searchPath);
    
    glview->setDesignResolutionSize(320, 568, ResolutionPolicy::FIXED_WIDTH);
    Director::getInstance()->setContentScaleFactor(glview->getFrameSize().width/320.0f);
    
    CCLOG("g:%f", glview->getFrameSize().width);
    CCLOG("d:%f", director->getWinSize().width);
    CCLOG("d:%f", director->getVisibleSize().width);
    CCLOG("gFactor:%f", glview->getContentScaleFactor());
    
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = EntryPoint::createScene();
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
