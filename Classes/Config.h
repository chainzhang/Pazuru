//
//  Config.h
//  pazuru
//
//  Created by 張程 on 2014/05/02.
//
//

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define TOTAL_BLOCK_NUM (5)

#define BLOCK_STAGE_WIDTH (8)
#define BLOCK_STAGE_HEIGHT (6)

enum gameState {
    GAME_STATE_NORMAL,
    GAME_STATE_ANIMATION_PLAYING,
    GAME_STATE_REMOVING_BALL,
    GAME_STATE_MOVEDOWN,
    GAME_STATE_RUNNING_PAZURU,
    GAME_STARE_PAUSE
};


static const char* ballImageName[TOTAL_BLOCK_NUM] = {
    "rectangle.png",
    "triangle.png",
    "circle.png",
    "xmark.png",
    "star.png"
};

#endif
