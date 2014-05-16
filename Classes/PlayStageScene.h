#ifndef __PLAY_STAGE_SCENE_H__
#define __PLAY_STAGE_SCENE_H__

#include "cocos2d.h"
#include "BallSprite.h"
#include "Config.h"

USING_NS_CC;


class PlayStage : public Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();
    PlayStage();
    ~PlayStage();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    // implement the "static create()" method manually
    CREATE_FUNC(PlayStage);
    
    virtual void update(float delta);
    
private:
    
    BallSprite **m_matrix;
    std::vector<int>BallToDelete;
    std::vector<int>BallToDeleteByUser;
    std::vector<int>BallList;
    
    std::vector<int>_actived_balls;
    Point _last_touch;
    
    Label *scoreLabel;
    int score;
    int current_score;
    unsigned int bounce = 2;
    int chains = 0;
    
    bool _pazuru;
    
    bool _ball_removed;
    bool _ball_filled;
    bool _ball_movedown_end;
    bool _animation_running;
    bool _user_contorl_end;
    bool _pazuru_done;
    bool _do_pazuru;
    
    int _times;
    
    float _time_to_move_down;
    
    
    int const max_num_to_select = 3;
    int num_selected;
    
    int m_width;
    int m_height;
    
    int m_matrix_x;
    int m_matrix_y;
    
    void initTouchEvent();
    void initBallStage(bool actived = false);
    void createAndDropBall(int row, int col, bool actived = false);
    void enableTouch();
    void disableTouch();
    void checkMoveDown();
    
    void checkIndex(int index);
    
    std::vector<int> checkInRow(int start_index);
    std::vector<int> checkInCol(int start_index);
    
    void pazuruProcess();
    void removeBallProcess(float delta);
    void movedownBallProcess(float delta);
    
    bool SelectBallTouchOn(Point position);
    void DeleteBall(std::vector<int>&balls, bool star_anime = false);
    EventListenerTouchOneByOne *_listener;
    Sprite *ballSheet;
    
    int ActionIsRunning();
    
    int _game_state;
    
    //Point positionOfItem(int row, int col);
};

#endif // __PlayStageScene_SCENE_H__
