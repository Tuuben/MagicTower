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
#define CAMERA_SPEED 60
public:
    static GameScene* getInstance();
    
    static cocos2d::Scene* createScene();
    
    static GameScene* create();
    
    virtual bool init();
    
    void update(float dt);
    
    void flashColor( cocos2d::Color3B color );
    
    Player* getPlayer();
    
private:
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
public:
    MapHandler* mapH;
    Player* playerObj = nullptr;
    FlashLayer* flashLayer;
    cocos2d::Vec2 lastPlayerPos;
    cocos2d::Size visibleSize;

private:
    static GameScene* _instance;
};

#endif // __GAMESCENE_SCENE_H__
