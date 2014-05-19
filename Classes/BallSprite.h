//
//  BlockSprite.h
//  pazuru
//
//  Created by 張程 on 2014/05/02.
//
//

#ifndef __pazuru__BlockSprite__
#define __pazuru__BlockSprite__

#include "cocos2d.h"


USING_NS_CC;

class BallSprite : public Sprite
{
public:
    bool selected;
    static BallSprite* create(int row, int col);
    
    CC_SYNTHESIZE(int, m_row, Row);
    CC_SYNTHESIZE(int, m_col, Col);
    CC_SYNTHESIZE(int, m_imgIndex, ImgIndex);
    
    CREATE_FUNC(BallSprite);
};

#endif /* defined(__pazuru__BlockSprite__) */
