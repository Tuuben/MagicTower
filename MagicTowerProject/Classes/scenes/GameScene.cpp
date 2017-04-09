#include "GameScene.h"
#include "Bat.h"

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
    
    mapH = MapHandler::create();
    float mapOffsetX = visibleSize.width - mapH->getMapWidth();
    mapH->setPositionX( mapOffsetX / 2 );
    addObject(mapH);
    
    playerObj = Player::create( getObjectLayer() );
    playerObj->setPosition(Vec2( visibleSize.width / 2, visibleSize.height / 2 ));
    addObject(playerObj);
    
    flashLayer = FlashLayer::create(FOOD_COLOR);
    flashLayer->setGlobalZOrder(100);
    this->addChild(flashLayer);
    
    // TEMP
    auto grimGraveStoneSpr = Sprite::createWithSpriteFrameName(GRIM_REAPER_GRAVESTONE_01);
    grimGraveStoneSpr->setPosition(Vec2( visibleSize.width / 2, 42  ));
    grimGraveStoneSpr->setGlobalZOrder(-80);
    addObject(grimGraveStoneSpr);
    
    
    this->scheduleUpdate();
    
    auto keyBoardEventListener = EventListenerKeyboard::create();
    keyBoardEventListener->onKeyPressed = CC_CALLBACK_2( GameScene::onKeyPressed, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardEventListener, this);
    
    return true;
}

void GameScene::update(float dt)
{
    World::update(dt);
    
    setCameraPositionX( MathUtil::lerp(getCameraPositionX(), 0, dt * 5) );
    setCameraPositionY( getCameraPositionY() + (CAMERA_SPEED * dt) );
    
//    setCameraPositionY( playerObj->getPositionY() -  (visibleSize.height / 2) );
    
    //check if should update map
    if( (getCameraPositionY() + visibleSize.height) >= mapH->getMapHeight() - 150)
    {
        float clearBelowPosY = getCameraPositionY(); //playerObj->getPositionY() - (visibleSize.height / 2);
        mapH->updateMap( clearBelowPosY );
    }

}

void GameScene::flashColor( Color3B color )
{
    flashLayer->setColor(color);
    flashLayer->flash(30, 0.14f);
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


