#include "PlayStageScene.h"

PlayStage::PlayStage():
_game_state(GAME_STATE_NORMAL),
_ball_filled(true),
_ball_removed(false),
_user_contorl_end(false),
_animation_running(false),
_ball_movedown_end(true),
_pazuru_done(false),
_do_pazuru(false),
_pazuru(false),
BallList(NULL),
BallToDelete(NULL),
_last_touch(-1,-1),
_listener(NULL),
ballSheet(NULL),
m_matrix(NULL),
num_selected(0),
m_width(0),
m_height(0),
m_matrix_x(0),
m_matrix_y(0),
_times(0),
score(0),
current_score(0)
{
    
}

PlayStage::~PlayStage()
{
    if (m_matrix) {
        free(m_matrix);
    }
}

Scene* PlayStage::createScene()
{
    // シーンを作成します。
    auto scene = Scene::create();
    
    // レイヤーを作成します。
    auto layer = PlayStage::create();
    
    // レイヤーをシーンに貼付けます。
    scene->addChild(layer);
    // シーンを返します。
    return scene;
}

// on "init" you need to initialize your instance
bool PlayStage::init()
{
    CCLOG("Called");
    if ( !Layer::init() )
    {
        return false;
    }
    
    this->initTouchEvent();
    
    Size screenSize = Director::getInstance()->getVisibleSize();
    _screenSize = screenSize;

    //background->setPosition();
    auto background = Sprite::create("background.png");
    background->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
    background->setPosition(Point(0, screenSize.height));
    this->addChild(background,0);
    
    CCLOG("Background width:%f", background->getContentSize().width);
    
    
    //UI
    auto PointLabel = Label::createWithBMFont("ui_font.fnt", "POINT:");
    PointLabel->setScale(0.8f);
    PointLabel->setPosition(Point(50, screenSize.height-20));
    
    this->addChild(PointLabel, 10);
    
    scoreLabel = Label::createWithBMFont("ui_font.fnt", "0");
    scoreLabel->setScale(0.8f);
    scoreLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    scoreLabel->setPosition(Point(20+PointLabel->getContentSize().width*.8, screenSize.height-20));


	ballSheetArea = ClippingNode::create();
	ballSheetArea->setStencil(LayerColor::create(Color4B(0, 0, 0, 255), _screenSize.width, 240.0f));
	ballSheetArea->setAlphaThreshold(255);
	ballSheetArea->setInverted(false);

    
    this->addChild(scoreLabel);
    
    

    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture_pazuru.plist", "texture_pazuru.pvr.ccz");
    
    ballSheet = Sprite::create();
	ballSheet->retain();
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, ballSheet);
    
    this->addChild(ballSheet,2);
    
    m_width = BLOCK_STAGE_WIDTH;
    m_height = BLOCK_STAGE_HEIGHT;
    
    m_matrix_x = 0;
    m_matrix_y = 0;
    
    int arraySize = sizeof(BallSprite *) * m_width * m_height;
    m_matrix = (BallSprite **)malloc(arraySize);
    memset((void*)m_matrix, 0, arraySize);
    
    this->scheduleUpdate();
    
    initBallStage();
    return true;
}

void PlayStage::initTouchEvent()
{
    _listener = EventListenerTouchOneByOne::create();
    
    _listener->onTouchBegan = [&](Touch *touch, Event *event){
        /*
         auto target = static_cast<Sprite*>(event->getCurrentTarget());
         
         Point locationInNode = target->convertToNodeSpace(touch->getLocation());
         //Size s = target->getContentSize();
         Rect rect = Rect(0, 0, 40.0f, 40.0f);
         
         if (rect.containsPoint(locationInNode))
         {
         target->setOpacity(160);
         return true;
         }
         */
        
        if (_animation_running || !_actived_balls.empty() || _game_state != GAME_STATE_NORMAL)
        {
            return false;
        }
        
        _last_touch = touch->getLocation();
        //CCLOG("%f - %f", touch->getLocation().x, touch->getLocation().y);
        
        
        this->SelectBallTouchOn(touch->getLocation());
        
        
        return true;
    };
    
    _listener->onTouchMoved = [&](Touch *touch, Event *event){
        
        //auto target = static_cast<Sprite*>(event->getCurrentTarget());
        
        //target->setPosition(target->getPosition()+touch->getDelta());
        this->SelectBallTouchOn(touch->getLocation());
        //CCLOG("%f - %f", touch->getLocation().x, touch->getLocation().y);
    };
    
    _listener->onTouchEnded = [=](Touch *touch, Event *event){
        
        if (num_selected == 0) return;
        
        CCLOG("touch end");
        this->disableTouch();
        bounce = 2;
        chains = 0;
        this->DeleteBall(BallToDeleteByUser);
        _user_contorl_end = true;
        
    };
}

void PlayStage::initBallStage(bool actived)
{
    //if (_game_state != GAME_STATE_NORMAL || _game_state != GAME_STATE_PAUSE) return;
	

    for (int r = 0; r < m_height; r++)
    {
        for (int c = 0; c < m_width; c++)
        {
            if (m_matrix[c+r*m_width] == 0)
                createAndDropBall(r, c, actived);
        }
    }
    if (!actived) {
        _game_state = GAME_STATE_NORMAL;
        _user_contorl_end = false;
        this->runAction(Sequence::create(DelayTime::create(0.1f),
                                         CallFunc::create(CC_CALLBACK_0(PlayStage::enableTouch, this)), NULL));
    }
}


void PlayStage::createAndDropBall(int row, int col, bool actived)
{
    Size screenSize = Director::getInstance()->getWinSize();
    
    auto ball = BallSprite::create(row, col);
    
    int x = 40*col+20;
    int y = 40*row+20;
    
    Point startPosition = Point(x, y+screenSize.height);
    Point endPosition = Point(x, y);
    
    Sequence *drop = Sequence::create(DelayTime::create(col*.05),
                                      EaseCubicActionIn::create(MoveTo::create(.15+row*.05,endPosition)),
                                      EaseCubicActionOut::create(MoveTo::create(.1f,Point(endPosition.x,endPosition.y+50.0f-row*5))),
                                      EaseCubicActionIn::create(MoveTo::create(.1f,endPosition)),
                                      EaseCubicActionOut::create(MoveTo::create(.05f,Point(endPosition.x,endPosition.y+3.0f))),
                                      EaseCubicActionIn::create(MoveTo::create(.03f,endPosition)),
                                      NULL);
    
    ball->setPosition(startPosition);
    /*
     if (row == 0 && col == 0) {
     _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, ball);
     } else {
     _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener->clone(), ball);
     }
     */
    
    ball->runAction(drop);

    ballSheet->addChild(ball,3);
    
    m_matrix[row * m_width + col] = ball;
    
    if (actived) {
        _actived_balls.push_back(row * m_width + col);
    }
    
}


bool PlayStage::SelectBallTouchOn(Point position)
{
    for (int i = 0 ; i < m_width*m_height; i++ )
    {
        if (m_matrix[i] != 0)
        {
            Point the_position = m_matrix[i]->getPosition();
            Rect safe_area = Rect(_last_touch.x - 39.8, _last_touch.y - 39.8, 79.6, 79.6);
            if (!safe_area.containsPoint(position)){
                return false;
            }
            
            Rect rect = Rect(the_position.x-19.8, the_position.y-19.8, 39.6, 39.6);
            if (rect.containsPoint(position) &&
                max_num_to_select > num_selected &&
                ! m_matrix[i]->selected)
            {
                m_matrix[i]->setOpacity(120);
                m_matrix[i]->selected = true;
                BallToDeleteByUser.push_back(i);
                num_selected++;
                CCLOG("BallTouched:%i", num_selected);
            }
            
            _last_touch = position;
        }
        
    }
    return false;
}

void PlayStage::enableTouch()
{
    _eventDispatcher->setEnabled(true);
}

void PlayStage::disableTouch()
{
    _eventDispatcher->setEnabled(false);
}

void PlayStage::DeleteBall(std::vector<int>&balls, bool star_anime)
{
    if (balls.empty()) return;
    
    while (m_matrix[balls.back()] == 0){
        balls.pop_back();
    }
    
    //Point first_point = m_matrix[balls.back()]->getPosition();
    int points = 0;
    
    while (!balls.empty()){
        if (m_matrix[balls.back()] == 0) {
            balls.pop_back();
            continue;
        }
		m_matrix[balls.back()]->runAction(Sequence::create(EaseBackInOut::create(ScaleTo::create(.3f, 0)),
														   CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, m_matrix[balls.back()], true)),
			NULL));
        
        if (star_anime){
            ballSheet->runAction(Sequence::create(DelayTime::create(0.16f),
											 CallFunc::create(CC_CALLBACK_0(PlayStage::boomAnime, this, m_matrix[balls.back()]->getPosition())),
                                             NULL));
        }

        m_matrix[balls.back()] = 0;
        points += bounce;
        
        //CCLOG("Ball is Removed:%i", balls.back());
        if (chains > 1 && bounce < 60)
            bounce += 20*(chains-1);
    }
   
    
    if (star_anime) {
        score+=points;
        /*
        auto particleStars = ParticleSystemQuad::create("star_boom_ver4.plist");
        particleStars->setAutoRemoveOnFinish(true);
        particleStars->setBlendAdditive(true);
        //particleStars->setScale(0.5f);
        particleStars->setPosition(first_point);
        this->addChild(particleStars, 300);
         */
        chains++;
    }
    
    CCLOG("Chains:%i", chains);
    CCLOG("%i",score);
    CCLOG("b:%i",bounce);
    
    _ball_removed = true;
    _ball_filled = false;
    _ball_movedown_end = false;
    
    num_selected = 0;
}







void PlayStage::checkMoveDown()
{
    //if (_do_pazuru) return;
    //_ball_movedown_end = false;
    if (_game_state == GAME_STATE_REMOVING_BALL || _game_state == GAME_STATE_RUNNING_PAZURU) return;
    _game_state = GAME_STATE_MOVEDOWN;
    
    int r;
    int height_to_drop;
    
    for (int c = 0; c < m_width; c++)
    {
        r = 0;
        height_to_drop = 0;
        while (r < m_height) {
            //CCLOG("Ball %i is 0", c+r*m_width);
            while (m_matrix[c+r*m_width] == 0) {
                height_to_drop++;
                r++;
            }
            
            if (height_to_drop > 0 && r < m_height){
                //CCLOG("GOING DOWN : %i", c+r*m_width);
                Point originPos = m_matrix[c+r*m_width]->getPosition();
                m_matrix[c+r*m_width]->runAction(EaseBackIn::create(MoveTo::create(0.3f, Point(originPos.x, originPos.y - height_to_drop*40.0f))));
                
                m_matrix[c+(r-height_to_drop)*m_width] = m_matrix[c+r*m_width];
                m_matrix[c+r*m_width] = 0;
                
                _actived_balls.push_back(c+(r-height_to_drop)*m_width);
                
            }
            r++;
        }
    }
    _game_state = GAME_STATE_NORMAL;
}


void PlayStage::pazuruProcess()
{
    //if (!_ball_movedown_end) return;
    if (_game_state == GAME_STATE_MOVEDOWN) return;
    if (_actived_balls.empty()) return;
    
    _game_state = GAME_STATE_RUNNING_PAZURU;
    
    std::vector<int>balls_to_check;
    
    while (!_actived_balls.empty())
    {
        //CCLOG("CheckBall:%i",actived_balls.back());
        balls_to_check = this->checkInRow(_actived_balls.back());
        balls_to_check.push_back(_actived_balls.back());
        BallList = balls_to_check;
        
        if (_pazuru) {
            //CCLOG("Pazuru open");
            std::vector<int>result;
            while (!balls_to_check.empty()) {
                result = checkInCol(balls_to_check.back());
                BallList.insert(BallList.end(), result.begin(), result.end());
                balls_to_check.pop_back();
            }
        }
        
        balls_to_check = this->checkInCol(_actived_balls.back());
        balls_to_check.push_back(_actived_balls.back());
        BallList.insert(BallList.end(),balls_to_check.begin(), balls_to_check.end());
        
        if (_pazuru) {
            //CCLOG("Pazuru open");
            std::vector<int>result;
            while (!balls_to_check.empty()) {
                result = checkInRow(balls_to_check.back());
                BallList.insert(BallList.end(), result.begin(), result.end());
                balls_to_check.pop_back();
            }
        } else {
            BallList.clear();
        }
        
        BallToDelete.insert(BallToDelete.end(), BallList.begin(), BallList.end());
        
        _pazuru = false;
        _actived_balls.pop_back();
    }
    
    if (BallToDelete.empty()){
        this->showAttackCount();
        
        _pazuru_done = true;
        _do_pazuru = false;
        _game_state = GAME_STATE_COUNT;
    }
    
    while (!BallList.empty()){
        //CCLOG("BallToRemove:%i",BallList.back());
        BallList.pop_back();
    }
    //CCLOG("------------------------------");
    _game_state = GAME_STATE_NORMAL;
    
}

void PlayStage::checkIndex(int index)
{
}

std::vector<int> PlayStage::checkInCol(int start_index)
{
    int ballIndex = m_matrix[start_index]->getImgIndex();
    int i = 0;
    int num_connected = 0;
    std::vector<int>indexs;
    
    //CCLOG("index:%i", start_index);
    
    i = start_index;
    while (i < m_width*(m_height-1)) {
        i += m_width;
        if (m_matrix[i] == 0) continue;
        if (m_matrix[i]->getImgIndex() == ballIndex)
        {
            indexs.push_back(i);
            num_connected++;
        }
        else
        {
            break;
        }
    }
    
    i = start_index;
    while (i > m_width) {
        i -= m_width;
        if (m_matrix[i] == 0) continue;
        if (m_matrix[i]->getImgIndex() == ballIndex)
        {
            indexs.push_back(i);
            num_connected++;
        }
        else
        {
            break;
        }
    }
    
    if (num_connected >= 2) {
        _pazuru = true;
    }
    
    return indexs;
}

std::vector<int> PlayStage::checkInRow(int start_index)
{
    int ballIndex = m_matrix[start_index]->getImgIndex();
    int i = 0;
    int num_connected = 0;
    std::vector<int>indexs;
    
    while (i < start_index%m_width) {
        i++;
        if (m_matrix[start_index - i] == 0) continue;
        if (m_matrix[start_index - i]->getImgIndex() == ballIndex)
        {
            indexs.push_back(start_index - i);
            num_connected++;
        }
        else
        {
            break;
        }
    }
    
    i = start_index;
    while (i%m_width != m_width-1) {
        i++;
        if (m_matrix[i] == 0) continue;
        if (m_matrix[i]->getImgIndex() == ballIndex)
        {
            indexs.push_back(i);
            num_connected++;
        }
        else
        {
            break;
        }
        
    }
    //CCLOG("Check in row => %i", num_connected);
    if (num_connected >= 2) {
        _pazuru = true;
    }
    
    return indexs;
}

int PlayStage::ActionIsRunning()
{
    int num = 1;
    
    for (int i = 0; i < m_width*m_height; i++)
    {
        if (m_matrix[i] == 0) continue;
        if ((num = m_matrix[i]->getNumberOfRunningActions())) break;
    }
    
    return num;
}





void PlayStage::showAttackCount()
{
    _game_state = GAME_STATE_PAUSE;
    if (chains > 1) {
    
		ballSheetMask = LayerColor::create(Color4B(0, 0, 0, 255), _screenSize.width, 240.0f);
		ballSheetMask->setAnchorPoint(Point::ANCHOR_MIDDLE);
		ballSheetMask->setOpacity(0);
    
    ballSheetMask->runAction(Sequence::create(FadeTo::create(0.4f,160.0f),
                                     DelayTime::create(2.0f),
                                     FadeOut::create(0.3f),
                                     CallFunc::create(CC_CALLBACK_0(PlayStage::initBallStage, this, false)),
									 CallFunc::create(CC_CALLBACK_0(Layer::removeAllChildrenWithCleanup, ballSheetMask, true)),
									 CallFunc::create(CC_CALLBACK_0(Layer::removeFromParentAndCleanup, ballSheetMask, true)),
                                     NULL));
	ballSheetMask->runAction(Sequence::create(DelayTime::create(0.2f),
                                     CallFunc::create(CC_CALLBACK_0(PlayStage::countAnime, this, Point(_screenSize.width/2, ballSheetMask->getContentSize().height/2))),
                                     DelayTime::create(1.1f),
                                     CallFunc::create(CC_CALLBACK_0(PlayStage::countAnimeBoom, this, Point(_screenSize.width/2, ballSheetMask->getContentSize().height/2))),
                                     NULL));
    
    ballSheet->addChild(ballSheetMask, 400, GameNode::GAMENODE_MASK);
	
    } else {
        this->initBallStage();
    }
}


void PlayStage::countAnime(Point position)
{
    
    //auto anime_sprite = Sprite::create();
	auto mask = ClippingNode::create();
	mask->setStencil(LayerColor::create(Color4B(0, 0, 0, 255), _screenSize.width, 240.0f));
	mask->setAlphaThreshold(255);
	mask->setInverted(false);
    
    auto count_anime = ParticleSystemQuad::create("Count_anime.plist");
    count_anime->setAutoRemoveOnFinish(true);
    count_anime->setPosition(position);
    
    //anime_sprite->addChild(count_anime);
    
	mask->addChild(count_anime);
	ballSheetMask->addChild(mask, 500);
}

void PlayStage::countAnimeBoom(Point position)
{
	auto mask = ClippingNode::create();
	mask->setStencil(LayerColor::create(Color4B(0, 0, 0, 255), _screenSize.width, 240.0f));
	mask->setAlphaThreshold(255);
	mask->setInverted(false);

    auto count_boom_anime = ParticleSystemQuad::create("count_boom_ver2.plist");
	count_boom_anime->setAutoRemoveOnFinish(true);
	count_boom_anime->setPosition(position);
	count_boom_anime->setLifeVar(0.2f);
    
    auto chainLabel = Label::createWithBMFont("ui_font.fnt", std::to_string(chains) + " Chains!!");
    chainLabel->setScale(0);
    chainLabel->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.4f, 2.0f)),
                                           EaseBackOut::create(ScaleTo::create(0.18f, 1.0f)),
                                           DelayTime::create(0.4f),
                                           FadeOut::create(0.3f),
										   CallFunc::create(CC_CALLBACK_0(Label::removeFromParentAndCleanup, chainLabel, true)),
										   NULL));
    chainLabel->setPosition(position);
    
    //anime_sprite->addChild(count_anime);
    
	mask->addChild(count_boom_anime);
	mask->addChild(chainLabel);
	ballSheetMask->addChild(mask, 500);
}



void PlayStage::boomAnime(Point position)
{
    auto particleStars = ParticleSystemQuad::create("star_boom_ver18.plist");
    particleStars->setAutoRemoveOnFinish(true);
    particleStars->setBlendAdditive(true);
    particleStars->setScale(0.5f+chains*.1f);
    particleStars->setLife(0.2f + chains*0.02f);
    particleStars->setSpeed(400.0f + chains*20.0f);
    particleStars->setPosition(position);
    ballSheet->addChild(particleStars, 300);
}

void PlayStage::update(float delta){
    
    if (_game_state == GAME_STATE_PAUSE || !_user_contorl_end) return;
    
    _animation_running = this->ActionIsRunning();
    
    if (!_animation_running)
    {
        this->pazuruProcess();
        this->checkMoveDown();
        
        if (_ball_removed && !_ball_filled)
        {
            this->initBallStage(true);
        }
        
        if (!BallToDelete.empty())
        {
            this->DeleteBall(BallToDelete, true);
        }
        
    }
    
    if (score > current_score) {
        current_score += (score-current_score)/60+1;
        scoreLabel->setString(std::to_string(current_score));
        scoreLabel->updateContent();
        if (scoreLabel->getNumberOfRunningActions() == 0)
        {
            Point the_point = scoreLabel->getPosition();
            scoreLabel->runAction(Sequence::create(MoveTo::create(0.1f, Point(the_point.x, the_point.y+5)),
                                                   MoveTo::create(0.1f, Point(the_point.x, the_point.y)),NULL));
        }
    }
    
    
}

