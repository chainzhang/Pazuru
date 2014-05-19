//
//  BeeLogoScreen.h
//  pazuru
//
//  Created by 張程 on 2014/05/19.
//
//

#ifndef __BEE_LOGO_SCREEN_H__
#define __BEE_LOGO_SCREEN_H__

#include "cocos2d.h"
#include "BeeTools.h"

BEE_NS_BEGIN

class LogoScreen
{
public:
    LogoScreen(cocos2d::Node *parent);
    ~LogoScreen();
    void create(std::string filename, float duration);
    void play(cocos2d::CallFunc * callback = nullptr);
    
private:
    cocos2d::Node * m_parent;
    std::vector<cocos2d::Sprite*>screens;
    std::vector<float>screen_durations;
    static float delay;
    
    
};


BEE_NS_END


#endif /* defined(__pazuru__BeeLogoScreen__) */
