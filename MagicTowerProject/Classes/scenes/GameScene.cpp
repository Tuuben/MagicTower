#include "GameScene.h"
#include "../misc/Background.h"
#include "../candypunk/utility/Utils.h"
//Debug
#include "../objects/Soul.h"

GameScene* GameScene::_instance = nullptr;

GameScene* GameScene::getInstance()
{
    return _instance;
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(0);
    scene->getPhysicsWorld()->setUpdateRate(12);
    scene->getPhysicsWorld()->setSubsteps(6);
    scene->getPhysicsWorld()->setSpeed(2);
    
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GameScene* GameScene::create()
{
    GameScene* _gameScene = new GameScene();
    _instance = _gameScene;
    
    if(_gameScene && _gameScene->init())
    {
        _gameScene->autorelease();
        return _gameScene;
    }
    
    CC_SAFE_DELETE(_gameScene);
    return NULL;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    bool debugDrawPhysics = true;
    if ( !World::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    playerObj = Player::create( getObjectLayer() );
    playerObj->setPosition(Vec2( visibleSize.width / 2, (visibleSize.height / 2) * 0.1f ));
    addObject(playerObj);
    
    mapH = MapHandler::create();
    float mapOffsetX = visibleSize.width - mapH->getMapWidth();
    mapH->setPositionX( mapOffsetX / 2 );
    mapH->updateMap( -10.0f );
    addObject(mapH);
    
    flashLayer = FlashLayer::create(FOOD_COLOR);
    flashLayer->setGlobalZOrder(100);
    this->addChild(flashLayer);
    
    // TEMP
    auto grimGraveStoneSpr = Sprite::createWithSpriteFrameName(GRIM_REAPER_GRAVESTONE_01);
    grimGraveStoneSpr->setPosition(Vec2( visibleSize.width / 2, 42  ));
    grimGraveStoneSpr->setGlobalZOrder(-80);
    addObject(grimGraveStoneSpr);
    
    //REMOVE
    auto soul = Soul::create();
    soul->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    addObject(soul);
    playerObj->addSoul(soul);
    
   // createBackground();
    
    this->scheduleUpdate();
    
    auto keyBoardEventListener = EventListenerKeyboard::create();
    keyBoardEventListener->onKeyPressed = CC_CALLBACK_2( GameScene::onKeyPressed, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardEventListener, this);
    
    return true;
}

void GameScene::update(float dt)
{
    
    World::update(dt);

    moveCamera(dt);
    
    //check if should update map
  /*  if( (getCameraPositionY() + visibleSize.height) >= mapH->getMapHeight() - 150)
    {
        float clearBelowPosY = getCameraPositionY(); //playerObj->getPositionY() - (visibleSize.height / 2);
        mapH->updateMap( clearBelowPosY );
    }*/

}

void GameScene::flashColor( Color3B color )
{
    flashLayer->setColor(color);
    flashLayer->flash(30, 0.14f);
}

void GameScene::createBackground(){

    auto bg1 = Background::create();
    bg1->setPosition(Vec2(0,0));
    addObject(bg1);
    
    auto bg2 = Background::create();
    bg2->setPosition(Vec2(0, bg1->getTopPos()));
    addObject(bg2);
    
    bg1->setOther(bg2);
    bg2->setOther(bg1);

}

Player* GameScene::getPlayer(){
    
    if(!playerObj){
        CCLOGERROR("Player object null");
    }
        
    return playerObj;
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    
    if( keyCode == EventKeyboard::KeyCode::KEY_R)
    {
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
    
}

void GameScene::moveCamera(float dt){

    setCameraPositionX( MathUtil::lerp(getCameraPositionX(), 0.0f, 4.0f * dt) );
    
    if(!cameraIsMoving){
        setCameraPositionY( MathUtil::lerp(getCameraPositionY(), cameraYPos, 4.0f * dt) );
    }
    
    if(playerObj->getPositionY() - 10 > visibleSize.height * roomsIndex){
        
        cameraIsMoving = true;
        
        float clearBelowY = visibleSize.height * (roomsIndex - 1);
        mapH->updateMap( clearBelowY );
        
        auto move = MoveTo::create(1.0f, Vec2(0, -visibleSize.height * roomsIndex));
        auto onComplete = CallFunc::create([this](){
            this->cameraIsMoving = false;
            
            playerObj->unfreeze();
            for( auto obj : mapH->getMovingObjects() ){
                obj->unfreeze();
            }
            
        });
        auto wait = DelayTime::create(ON_ROOM_CHANGE_DELAY);
        getObjectLayer()->runAction( Sequence::create(EaseSineInOut::create(move), wait, onComplete, NULL));
        
        cameraYPos = visibleSize.height * roomsIndex;
        
        // Freeze moving objects
        playerObj->freeze();
        for( auto obj : mapH->getMovingObjects() ){
            obj->freeze();
        }
        
        roomsIndex++;
        
    }
    
}


