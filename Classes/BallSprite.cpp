//
//  BlockSprite.cpp
//  pazuru
//
//  Created by 張程 on 2014/05/02.
//
//

#include "BallSprite.h"
#include "Config.h"

BallSprite* BallSprite::create(int row, int col)
{
    auto *ball = new BallSprite();
    
    ball->selected = false;
    ball->m_row = row;
    ball->m_col = col;
    ball->m_imgIndex = arc4random()%TOTAL_BLOCK_NUM;
    
    ball->initWithSpriteFrameName(ballImageName[ball->m_imgIndex]);
    
    return ball;
}
