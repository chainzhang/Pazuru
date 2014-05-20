//
//  EntryPoint.h
//  pazuru
//
//  Created by 張程 on 2014/05/19.
//
//

#ifndef __ENTRY_POINT_SCENE__
#define __ENTRY_POINT_SCENE__

#include "BeeTools/BeeLogoScreen.h"

class PlayStage;

USING_NS_CC;

class EntryPoint : public Layer
{
public:
    
    static Scene * createScene();
    virtual bool init();
    virtual void onExit();
    
    CREATE_FUNC(EntryPoint);
    
private:
    Scene* _scene;
};


#endif /* defined(__pazuru__EntryPoint__) */
