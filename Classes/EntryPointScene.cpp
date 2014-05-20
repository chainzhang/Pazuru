//
//  EntryPoint.cpp
//  pazuru
//
//  Created by 張程 on 2014/05/19.
//
//

#include "EntryPointScene.h"
#include "PlayStageScene.h"
#include "BeeTools/BeeLogoScreen.h"


Scene * EntryPoint::createScene()
{
    auto scene = Scene::create();
    auto layer = EntryPoint::create();
    scene->addChild(layer);
    return scene;
}

bool EntryPoint::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto scene = PlayStage::createScene();
    scene->retain();
    
    Bee::LogoScreen *logoScreen = Bee::LogoScreen::create();
    logoScreen->add("background.png", 2.0f);
    logoScreen->add("background.png", 1.0f);
    logoScreen->play(CallFunc::create(CC_CALLBACK_0(Director::replaceScene, Director::getInstance(), scene)));
    this->addChild(logoScreen);
    
    //Sprite *background = Sprite::create("background.png");
    //this->addChild(background);
    
    return true;
}

void EntryPoint::onExit()
{
    Layer::onExit();
}