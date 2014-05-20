//
//  BeeLogoScreen.cpp
//  pazuru
//
//  Created by 張程 on 2014/05/19.
//
//

#include "BeeLogoScreen.h"

USING_NS_BEE;
USING_NS_CC;

float LogoScreen::delay = 0.0f;

LogoScreen::LogoScreen()
{
}

LogoScreen::~LogoScreen()
{
    this->removeAllChildrenWithCleanup(true);
}

LogoScreen* LogoScreen::create()
{
    static LogoScreen* instance = new LogoScreen();
    if (instance && instance->init())
    {
        instance->autorelease();
        return instance;
    }
    CC_SAFE_DELETE(instance);
    return NULL;
}

void LogoScreen::add(std::string filename, float duration)
{
    Size screenSize = Director::getInstance()->getVisibleSize();
    
    Sprite *screen = Sprite::create(filename);
    screen->setAnchorPoint(Point::ANCHOR_MIDDLE);
    screen->setPosition(screenSize.width/2,screenSize.height/2);
    screen->setOpacity(0);
    
    screens.push_back(screen);
    screen_durations.push_back(duration);
}

void LogoScreen::play(CallFunc *callback)
{
    while (!screens.empty())
    {
        
        CCLOG("OK");
        
        Vector<FiniteTimeAction*>actions;
        actions.pushBack(DelayTime::create(delay));
        actions.pushBack(EaseSineIn::create(FadeIn::create(1.0f)));
        actions.pushBack(DelayTime::create(screen_durations.back()));
        actions.pushBack(EaseSineOut::create(FadeOut::create(1.0f)));
        actions.pushBack(CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, screens.back(),true)));
        
        if (screens.size() == 1 && callback != nullptr)
            actions.pushBack(callback);
        
        screens.back()->runAction(Sequence::create(actions));
        this->addChild(screens.back());
        
        delay+= screen_durations.back()+2.0f;
        
        screens.pop_back();
        screen_durations.pop_back();
    }
}