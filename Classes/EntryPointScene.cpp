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

EntryPoint::EntryPoint()
{
    
}

EntryPoint::~EntryPoint()
{
    
}

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
    
    
    Bee::LogoScreen *logoScreen = new Bee::LogoScreen();
    this->addChild(logoScreen->create("background.png", 2.0f));
    logoScreen->play(CallFunc::create(CC_CALLBACK_0(Director::replaceScene, Director::getInstance(),scene)));
    
    //Sprite *background = Sprite::create("background.png");
    //this->addChild(background);
    
    
    return true;
}

void EntryPoint::onExit()
{
    
}

void EntryPoint::onEnterTransitionDidFinish()
{
    
}

void EntryPoint::onEnter()
{
    //auto scene = PlayStage::createScene();
    //Director::getInstance()->replaceScene(scene);
}