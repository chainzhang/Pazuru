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

USING_NS_CC;

class EntryPoint : public Layer
{
public:
    EntryPoint();
    ~EntryPoint();
    
    static Scene * createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
    
    CREATE_FUNC(EntryPoint);
};


#endif /* defined(__pazuru__EntryPoint__) */
