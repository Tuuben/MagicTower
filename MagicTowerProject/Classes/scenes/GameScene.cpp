#include "GameScene.h"
#include "../misc/Background.h"
#include "../candypunk/utility/Utils.h"
//Debug
#include "../objects/Soul.h"
#include "../objects/SimpleParticle.h"

GameScene* GameScene::_instance = nullptr;

GameScene* GameScene::getInstance()
{
    return _instance;
}

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(1);
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
    playerObj->setPosition(Vec2( visibleSize.width / 2, 8.0f ));
    playerObj->freeze();
    addObject(playerObj);
    
    mapH = MapHandler::create();
    float mapOffsetX = visibleSize.width - mapH->getMapWidth();
    mapH->setPositionX( mapOffsetX / 2 );
    mapH->updateMap( -10.0f );
    addObject(mapH);
    
    flashLayer = FlashLayer::create(FOOD_COLOR);
    flashLayer->setGlobalZOrder(100);
    this->addChild(flashLayer);
    
    /// TEMP _ SET PLAYER STATS

    ///
    
    setupEvents();
    createDithering();
    createInitalRoomObjects();
    createScoreLabel();
    createHealthBar();
    
    this->scheduleUpdate();
    
    auto keyBoardEventListener = EventListenerKeyboard::create();
    keyBoardEventListener->onKeyPressed = CC_CALLBACK_2( GameScene::onKeyPressed, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyBoardEventListener, this);
    
    return true;
}

void GameScene::setupEvents(){

    auto eventTouch = EventListenerTouchOneByOne::create();
    eventTouch->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventTouch, this);
    
}

void GameScene::update(float dt)
{
    
    World::update(dt);

    moveCamera(dt);
    
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

HealthBar* GameScene::getHealthBar(){

    return healthBar;
    
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    
    if( keyCode == EventKeyboard::KeyCode::KEY_R)
    {
        auto scene = GameScene::createScene();
        Director::getInstance()->replaceScene(scene);
    }
    
}

bool GameScene::onTouchBegan(Touch *touch, Event* event){
    
    if(!firstTap){
        startGameCountdown();
        firstTap = true;
    }
    
    if(!gameActive){
        emitStoneParticles();
        addScore(10);
        setScreenShake(0.2f, 6.0f, 6.0f);
    }
    
    return true;
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
        
        // Add score
        addScore(20);
        
    }
    
}

void GameScene::createDithering(){

    float baseX = 0;//-visibleSize.width / 2;
    for(int i = 0; i < 20; i++){
        
        Vec2 sPos = Vec2(baseX + (i * 16), visibleSize.height - 8);
        auto sTop = Sprite::createWithSpriteFrameName(DITHERING_TILE);
        sTop->setPosition(sPos);
        sTop->setGlobalZOrder(150);
        addChild(sTop);
        
        sPos.y = 8.0f;
        auto sBottom = Sprite::createWithSpriteFrameName(DITHERING_TILE);
        sBottom->setScaleY(-1.0f);
        sBottom->setPosition(sPos);
        sBottom->setGlobalZOrder(150);
        addChild(sBottom);

    }
    
}

void GameScene::createInitalRoomObjects(){

    gameStartLabel = Label::createWithTTF("Tap if you wish to live ...", "fonts/alagard.ttf", 18);
    gameStartLabel->getFontAtlas()->setAliasTexParameters();
    gameStartLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    addObject(gameStartLabel);
    
    gameStartCountDownLabel = Label::createWithTTF("", "fonts/alagard.ttf", 18);
    gameStartCountDownLabel->getFontAtlas()->setAliasTexParameters();
    gameStartCountDownLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.45f));
    addObject(gameStartCountDownLabel);
    
}

void GameScene::createScoreLabel(){

    scoreLabel = Label::createWithTTF("00000000", "fonts/alagard.ttf", 18);
    scoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.96f));
    scoreLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
    scoreLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    scoreLabel->setGlobalZOrder(140);
    addChild(scoreLabel);
    
    auto bg = LayerColor::create(Color4B::BLACK, visibleSize.width, 42);
//    bg->drawSolidRect(Vec2(-60, -10), Vec2(60, 10), Color4F::BLACK);
//    drawBg->setPosition(Vec2( 60, 10 ));
    bg->setPosition(Vec2(0.0f, visibleSize.height * 0.95f));
    bg->setGlobalZOrder(135);
    addChild(bg);
    
}

void GameScene::createHealthBar(){
    
    healthBar = HealthBar::create();
    healthBar->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.93f ));
    addChild(healthBar);
    
}

void GameScene::createTutorialSection(){

    float fadeTime = 0.4f;
    
    for(int i = -5; i < 6; i++){
    
        auto lineSpr = Sprite::createWithSpriteFrameName(TUTORIAL_LINE);
        lineSpr->setPosition(Vec2( visibleSize.width / 2, visibleSize.height / 2 + (16 * i) ));
        lineSpr->setOpacity(0);
        lineSpr->runAction(FadeTo::create(fadeTime, 255));
        addObject(lineSpr);
        
    }
    
    auto leftArrow = Sprite::createWithSpriteFrameName(TUTORIAL_ARROW);
    leftArrow->setScaleX(-1);
    leftArrow->setPosition(Vec2(visibleSize.width * 0.30f, visibleSize.height * 0.6f));
    leftArrow->setOpacity(0);
    addObject(leftArrow);
    
    auto leftText = Label::createWithTTF("TAP LEFT", "fonts/alagard.ttf", 16);
    leftText->setAlignment(TextHAlignment::CENTER);
    leftText->setPosition(Vec2( visibleSize.width * 0.30f, visibleSize.height * 0.5f ));
    leftText->setOpacity(0);
    addObject(leftText);
    
    auto rightArrow = Sprite::createWithSpriteFrameName(TUTORIAL_ARROW);
    rightArrow->setPosition(Vec2(visibleSize.width * 0.7f, visibleSize.height * 0.6f));
    rightArrow->setOpacity(0);
    addObject(rightArrow);
    
    auto rightText = Label::createWithTTF("TAP RIGHT", "fonts/alagard.ttf", 16);
    rightText->setAlignment(TextHAlignment::CENTER);
    rightText->setPosition(Vec2( visibleSize.width * 0.7f, visibleSize.height * 0.5f ));
    rightText->setOpacity(0);
    addObject(rightText);
    

    leftText->runAction(FadeTo::create(fadeTime, 255));
    rightText->runAction(FadeTo::create(fadeTime, 255));
    rightArrow->runAction(FadeTo::create(fadeTime, 255));
    leftArrow->runAction(FadeTo::create(fadeTime, 255));
    
}

void GameScene::startGameCountdown(){

    // Countdown label effect
    int val = 3;
    this->gameStartCountDownLabel->setString("3");
    
    auto countDownFunc = CallFunc::create([this, val]() mutable {
        val--;
        if(val > 0){
            std::stringstream stream;
            stream << val;
            this->gameStartCountDownLabel->setString(stream.str());
        }
        else{
            this->gameStartCountDownLabel->setString("");
        }
    });
    
    auto wait = DelayTime::create(1.0f);
    auto seq = Sequence::create(wait, countDownFunc, NULL);
    runAction(Repeat::create(seq, 3));
    
    // Start game
    auto delayStart = DelayTime::create(startGameDelayTime);
    auto callFunc = CallFunc::create([this](){
        
        playerObj->setPosition(Vec2(visibleSize.width / 2, 32));
        playerObj->unfreeze();
        this->gameActive = true;
        mapH->getReaperStone()->breakStone();
        this->setScreenShake(0.8f, 16.0f, 16.0f);
        
//        for(int i = 0; i < 12; i++)
//            this->emitStoneParticles();
        
        for(int i = 0; i < 12; i++){
            
            float angle = MATH_PIOVER2 * 4.0f * (i / 12.0f);
            float vx = cos(angle) * 30.0f;
            float vy = sin(angle) * 30.0f;
            float lifetime = 1.0f;
            auto part = SimpleParticle::create(PARTICLE_CIRCLE_01, vx, vy, lifetime, 3.0f, 0.0f);
            part->setGlobalZOrder(-100);
            part->setPosition( Vec2( (visibleSize.width * 0.495f) + (cos(angle) * 5.0f), (visibleSize.height * 0.05f) + (sin(angle) * 5.0f) ));
            part->setIgnoreGravity(true);
            this->addObject(part);
            
        }
        
        for(int i = 0; i < 12; i++){
            
            float angle = MATH_PIOVER2 * 4.0f * (i / 12.0f);
            float vx = cos(angle) * 240.0f;
            float vy = sin(angle) * 240.0f;
            float lifetime = 1.0f;
            auto part = SimpleParticle::create(PARTICLE_CIRCLE_01, vx, vy, lifetime, 2.0f, 0.0f);
            part->setGlobalZOrder(-100);
            part->setPosition( Vec2( (visibleSize.width * 0.495f) + (cos(angle) * 5.0f), (visibleSize.height * 0.05f) + (sin(angle) * 5.0f) ));
            part->setIgnoreGravity(true);
            this->addObject(part);
            
        }
        
        this->createTutorialSection();
        
        this->gameStartLabel->setVisible(false);
        
    });
    runAction(Sequence::create(delayStart, callFunc, NULL));
    
}

void GameScene::emitStoneParticles(){
    
    for(int i = 0; i < 12; i++){
        
        float vx = 120.0f * CCRANDOM_0_1() - 60.0f;
        float vy = 300.0f * CCRANDOM_0_1();
        float lifeTime = 2.0f * CCRANDOM_0_1() + 0.5f;
        float startScale = 0.0f * CCRANDOM_0_1() + 1.0f;
        float endScale = 0.0f;
        std::string spriteFrameNames[] = {PARTICLE_STONE_01, PARTICLE_STONE_02, PARTICLE_STONE_03, PARTICLE_STONE_04};
        auto part = SimpleParticle::create(spriteFrameNames[(int)(4 * CCRANDOM_0_1())], vx, vy, lifeTime, startScale, endScale);
        part->setPosition(Vec2( visibleSize.width / 2, 0.0f ));
        part->setTorque(360.0f * CCRANDOM_0_1() * ((i % 2 == 1) ? -1 : 1));
        addChild(part);
        
    }
    
}

void GameScene::addScore(int amt){
    
    score += amt;
    
    CCLOG("score: %d", score);
    std::stringstream stream;

    // 00000000
    if(score < 10){
        stream << "0000000" << score;
    }
    if(score >= 10 && score < 100){
        stream << "000000" << score;
    }
    if(score >= 100 && score < 1000){
        stream << "00000" << score;
    }
    if(score >= 1000 && score < 10000){
        stream << "0000" << score;
    }
    if(score >= 10000 && score < 100000){
        stream << "0000" << score;
    }
    if(score >= 100000 && score < 1000000){
        stream << "000" << score;
    }
    if(score >= 1000000 && score < 10000000){
        stream << "00" << score;
    }
    if(score >= 10000000 && score < 100000000){
        stream << "0" << score;
    }
    if(score >= 100000000){
        stream << "" << score;
    }
    
    scoreLabel->setString(stream.str());

}



