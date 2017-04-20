#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "../Globals.h"
#include "../handlers/MapHandler.h"
#include "../objects/Player.h"
#include "../candypunk/World.h"
#include "../objects/FlashLayer.h"

USING_NS_CC;

class GameScene : public World
{
#define MIN_CAMERA_POS 10
#define ON_ROOM_CHANGE_DELAY 0.3f
public:
    static GameScene* getInstance();
    
    static cocos2d::Scene* createScene();
    
    static GameScene* create();
    
    virtual bool init();
    void setupEvents();
    void update(float dt);
    void flashColor( cocos2d::Color3B color );
    void createBackground();
    void createDithering();
    void createScoreLabel();
    void createInitalRoomObjects();
    void createTutorialSection();
    void startGameCountdown();
    void emitStoneParticles();
    void addScore(int amt);
    void setGameActive(bool active){ gameActive = active; };
    Player* getPlayer();
    
private:
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    bool onTouchBegan(Touch* touch, Event* event);
    void moveCamera(float dt);
    
public:
    MapHandler* mapH;
    Player* playerObj = nullptr;
    FlashLayer* flashLayer;
    cocos2d::Vec2 lastPlayerPos;
    cocos2d::Size visibleSize;
    bool gameActive = false;
    bool firstTap = false;

private:
    static GameScene* _instance;
    int roomsIndex = 1;
    int score = 0;
    bool cameraIsMoving = false;
    float cameraYPos = 0.0f;
    float startGameDelayTime = 3.0f;
    Label* gameStartCountDownLabel;
    Label* gameStartLabel;
    Label* scoreLabel;
    
};

#endif // __GAMESCENE_SCENE_H__
