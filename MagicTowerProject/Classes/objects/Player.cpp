//
//  Player.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 13-02-2016.
//
//

#include "Player.h"
#include "../scenes/GameScene.h"
#include "../handlers/AudioManager.h"
#include "../candypunk/utility/Utils.h"
#include "../PlayerStats.h"
#include "Soul.h"

Player* Player::create( cocos2d::Layer* onLayer )
{
    Player* _player = new Player();
    
    if(_player && _player->init(onLayer))
    {
        _player->autorelease();
        return _player;
    }
    
    CC_SAFE_DELETE(_player);
    return NULL;
}

bool Player::init( cocos2d::Layer* onLayer )
{
    _visibleSize = Director::getInstance()->getVisibleSize();
    _onLayer = onLayer;
    
    //create sprite
    _sprite = Sprite::createWithSpriteFrameName("blocky_idle_01.png");
    _sprite->getTexture()->setAliasTexParameters();
    _sprite->setGlobalZOrder(-20);
    _sprite->setColor(Color3B( 255, 255, 255 ));
    this->addChild(_sprite);

    //set sprite to animation comp
    setupAnimations();
    _animComponent = AnimationComponent::createComponent(_sprite);
    _animComponent->addAnimation(IDLE_ANIMATION_NAME, _idleAnimationFrames);
    _animComponent->playAnimation(IDLE_ANIMATION_NAME, 16, true);
    this->addComponent(_animComponent);
    
    //create physics body
    auto pBody = PhysicsBody::createCircle( 4 , PhysicsMaterial(1.0f, 0.0f, 1.0f) );
    pBody->setPositionOffset(Vec2(1, 0));
    pBody->setRotationEnable(false);
    pBody->setCollisionBitmask(PLAYER_BITMASK);
    pBody->setContactTestBitmask(true);
    pBody->setEnabled(true);
    this->setPhysicsBody(pBody);
    
    setupEvents();
    setStats();
    
    this->scheduleUpdate();
    
    return true;
}

void Player::removeHealth(){

    _lives--;
    _immortalDuration = IMMORTAL_DURATION;
    _immortal = true;
    
    GameScene::getInstance()->getHealthBar()->removeHeart();
    
    // Effect
    auto fadeTo = FadeTo::create(0.0f, 0);
    auto wait = DelayTime::create(IMMORTAL_DURATION / 30);
    auto fadeFrom = FadeTo::create(0.0f, 255);
    auto seq = Sequence::create(fadeTo, wait, fadeFrom, wait, NULL);
    _sprite->runAction(Repeat::create(seq, 15));
    
    freeze();
    
    auto cBack = CallFunc::create([this](){
        this->unfreeze();
    });
    
    runAction(Sequence::create(DelayTime::create(0.3f), cBack, NULL));
    
    if(_lives <= 0){
        kill();
    }
    
}

void Player::addSoul(Node* soul){
    _soulList.push_back(soul);
}

void Player::freeze(){
    
    Actor::freeze();
    for(auto sNode : _soulList){
        auto soul = dynamic_cast<Soul*>(sNode);
        soul->freeze();
    }

}

void Player::unfreeze(){
    
    Actor::unfreeze();
    for(auto sNode : _soulList){
    
        auto soul = dynamic_cast<Soul*>(sNode);
        soul->unfreeze();
        
    }
    
}


void Player::setupEvents()
{
    auto touchEventListener = EventListenerTouchAllAtOnce::create();
    touchEventListener->onTouchesBegan = CC_CALLBACK_2(Player::onTouchesBegan, this);
    
    auto collisionEventListener = EventListenerPhysicsContact::create();
    collisionEventListener->onContactBegin = CC_CALLBACK_1(Player::onContactBegin, this);
    collisionEventListener->onContactSeparate = CC_CALLBACK_1(Player::onContactSeparate, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(collisionEventListener, this);
}

void Player::setStats(){

    _lives = PlayerStats::getInstance()->getLives();
    
}

float speedLineScale = 0;
void Player::update(float dt)
{
    if(isFrozen() || _isDead)
        return;
    
    Actor::update(dt);
    
    float vy = this->getPhysicsBody()->getVelocity().y;
    if(vy <= -0.1f) {
        _touchBottom = false;
    }
    
    checkSideCollisions();

    if(_immortal)
        immortalCountdown(dt);
    
}

void Player::jump()
{
    Vec2 v = getPhysicsBody()->getVelocity();
    v.y = JUMP_FORCE;
    v.x = HORIZONTAL_SPEED * _direction;
    getPhysicsBody()->setVelocity(v);

    auto rotate = RotateTo::create(0.3f, 20 * _direction, 20 * _direction);
    auto rotate_back = RotateTo::create(0.2f, 0, 0);
    _sprite->runAction( Sequence::create(rotate, rotate_back, NULL) );
    
    float pitch = 0.4f * CCRANDOM_0_1() + 1.0f;
    AudioManager::getInstance()->playSoundEffect(SFX_JUMP_01, 1.0f, pitch);
    
    _touchingLeft = false;
    _touchingRight = false;
    _touchBottom = false;
    _touchTop = false;
    
}

void Player::immortalCountdown(float dt){

    _immortalDuration -= dt;
    CCLOG("Immortal %f", _immortalDuration);
    // Reset
    if(_immortalDuration <= 0.0f){
        _immortal = false;
        _immortalDuration = 0.0f;
    }
    
}

void Player::kill(){

    GameScene::getInstance()->setScreenShake(0.2f, 30.0f, 30.0f);
    
    this->setVisible(false);
    auto skull = Sprite::createWithSpriteFrameName(SKULL_01);
    auto pBody = PhysicsBody::createCircle(6);
    pBody->setVelocity(Vec2( 400.0f * CCRANDOM_0_1() - 200.0f, 200.0f * CCRANDOM_0_1() + 150.0f ));
    skull->addComponent(pBody);
    skull->setPosition(getPosition());
    getParent()->addChild(skull);
    
    for(int i = 0; i < 12; i++){
        
        float angle = MATH_PIOVER2 * 4.0f * (i / 12.0f);
        float vx = cos(angle) * 30.0f;
        float vy = sin(angle) * 30.0f;
        float lifetime = 1.0f;
        auto part = SimpleParticle::create(PARTICLE_CIRCLE_01, vx, vy, lifetime, 2.0f, 0.0f);
        part->setGlobalZOrder(100);
        part->setPosition( Vec2( getPositionX() + (cos(angle) * 5.0f), getPositionY() + (sin(angle) * 5.0f) ));
        part->setIgnoreGravity(true);
        getParent()->addChild(part);
        
    }
    
    for(int i = 0; i < 10; i++){
        
        float vx = 300.0f * CCRANDOM_0_1() - 150.0f;
        float vy = 300.0f * CCRANDOM_0_1() - 150.0f;
        float lifetime = 1.0f * CCRANDOM_0_1() + 0.6f;
        float startSize = 1.0f * CCRANDOM_0_1() + 0.6f;
        auto part = SimpleParticle::create(PARTICLE_DUST_01, vx, vy, lifetime, startSize, 0.0f);
        part->setGlobalZOrder(100);
        part->setPosition( Vec2( getPositionX(), getPositionY() ));
        part->setIgnoreGravity(true);
        getParent()->addChild(part);
        
    }
    
    _isDead = true;
}

void Player::checkSideCollisions()
{
    
    PhysicsQueryRectCallbackFunc collideLeft = [this](PhysicsWorld& world, PhysicsShape& shape, void* userData)->bool
    {
        if(shape.getBody()->getCollisionBitmask() == SOLID_BITMASK)
        {
            if ( !_touchingLeft )
            {
                auto colNode = shape.getBody()->getNode();
                Vec2 worldPos = colNode->getPosition() + colNode->getParent()->getPosition();
                worldPos.x += 8 * CCRANDOM_0_1() + 4;
                worldPos.y -= 8 * CCRANDOM_0_1();
                this->collisionEmit(worldPos, 1, -1);
                
                GameScene::getInstance()->setScreenShake(0.1f, 2.0f, 0.0f);
                
                _touchingLeft = true;
            }
        }
        
        return true;
    };
    
    PhysicsQueryRectCallbackFunc collideRight = [this](PhysicsWorld& world, PhysicsShape& shape, void* userData)->bool
    {
        if(shape.getBody()->getCollisionBitmask() == SOLID_BITMASK)
        {
            if( !_touchingRight )
            {
                auto colNode = shape.getBody()->getNode();
                Vec2 worldPos = colNode->getPosition() + colNode->getParent()->getPosition();//colNode->getParent()->convertToWorldSpace( colNode->getPosition() );
                worldPos.x -= 8 * CCRANDOM_0_1() + 4;
                worldPos.y -= 8 * CCRANDOM_0_1();
                this->collisionEmit(worldPos, -1, -1);
                
                GameScene::getInstance()->setScreenShake(0.1f, 2.0f, 0.0f);
                
                _touchingRight = true;
            }
        }
        
        return true;
    };
    
    PhysicsQueryRectCallbackFunc collideTop = [this](PhysicsWorld& world, PhysicsShape& shape, void* userData)->bool
    {
        if(shape.getBody()->getCollisionBitmask() == SOLID_BITMASK)
        {
            if( !_touchTop )
            {
                Vec2 worldPos( this->getPositionX(), this->getPositionY() + (PLAYER_CONTENT_SIZE.height) ); //colNode->getPosition() + colNode->getParent()->getPosition();
                
                this->getPhysicsBody()->setVelocity(Vec2( this->getPhysicsBody()->getVelocity().x, 0 ));
                
                GameScene::getInstance()->setScreenShake(0.1f, 2.0f, 0.0f);
                
                for(int i = 0; i < 10; i++)
                {
                    float vx = 60 * CCRANDOM_0_1() - 30.0f;
                    float vy = 50 * CCRANDOM_0_1();
                    float startScale = 0.6f * CCRANDOM_0_1() + 1.0f;
                    float endScale = 0;
                    float lifeTime = 0.2f * CCRANDOM_0_1() + 0.2f;
                    SimpleParticle* particle = SimpleParticle::create(PARTICLE_CIRCLE_01, vx, vy, lifeTime, startScale, endScale);
                    particle->setPosition(worldPos);
                    _onLayer->addChild(particle);
                }
                
                _touchTop = true;
            }
        }
        
        return true;
    };
    
    PhysicsQueryRectCallbackFunc collideBottom = [this](PhysicsWorld& world, PhysicsShape& shape, void* userData)->bool
    {
        if(shape.getBody()->getCollisionBitmask() == SOLID_BITMASK)
        {
            if( !_touchBottom )
            {
                Vec2 worldPos( this->getPositionX(), this->getPositionY() - (PLAYER_CONTENT_SIZE.height + 6) ); //colNode->getPosition() + colNode->getParent()->getPosition();
                worldPos.y += 8;
                
                for(int i = 0; i < 10; i++)
                {
                    float vx = 60 * CCRANDOM_0_1() - 30.0f;
                    float vy = 50 * CCRANDOM_0_1();
                    float startScale = 0.6f * CCRANDOM_0_1() + 1.0f;
                    float endScale = 0;
                    float lifeTime = 0.2f * CCRANDOM_0_1() + 0.2f;
                    SimpleParticle* particle = SimpleParticle::create(PARTICLE_CIRCLE_01, vx, vy, lifeTime, startScale, endScale);
                    particle->setPosition(worldPos);
                    _onLayer->addChild(particle);
                }
                
                _touchBottom = true;
            }
        }
        
        
        
        return true;
    };
    
   // bool bottom = Utils::rayCastDirection(getParent()->convertToWorldSpace(getPosition()), Vec2(-30, 0), SOLID_BITMASK, true);
    //CCLOG("left %d", bottom);
    
    Vec2 playerWorldPos = this->getParent()->convertToWorldSpace(this->getPosition());
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->queryRect(collideLeft, Rect( playerWorldPos.x - (PLAYER_CONTENT_SIZE.width / 2), playerWorldPos.y ,5, 2), nullptr);
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->queryRect(collideRight, Rect( playerWorldPos.x + (PLAYER_CONTENT_SIZE.width / 2), playerWorldPos.y ,5, 2), nullptr);
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->queryRect(collideTop, Rect( playerWorldPos.x, playerWorldPos.y + (PLAYER_CONTENT_SIZE.width / 2), 2, 2), nullptr);
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->queryRect(collideBottom, Rect( playerWorldPos.x, playerWorldPos.y - (PLAYER_CONTENT_SIZE.width / 2), 2, 2), nullptr);
}

bool Player::onTouchesBegan(std::vector<Touch*> touches, cocos2d::Event *event)
{
    if(_isDead)
        return false;
    
    for(auto touch : touches){
        
        float touchXPos = touch->getLocation().x;
        if(touchXPos >= _visibleSize.width / 2)
        {
            _direction = 1;
        }
        else
        {
            _direction = -1;
        }
        
        jump();
        
        // If it is the initial jump
//        if(_isWaiting){
//            GameScene::getInstance()->setGameActive(true);
//            _isWaiting = false;
//        }
        
    }
    
    return true;
}

bool Player::onContactBegin(cocos2d::PhysicsContact &contact)
{
    auto shapeA = contact.getShapeA();
    auto shapeB = contact.getShapeB();
    
    if( (shapeA->getCollisionBitmask() == PLAYER_BITMASK && shapeB->getCollisionBitmask() == SOLID_BITMASK) ||
        (shapeB->getCollisionBitmask() == PLAYER_BITMASK && shapeA->getCollisionBitmask() == SOLID_BITMASK) )
    {
        if(_collisionCount <= 0)
        {
            float pitch = 0.4f * CCRANDOM_0_1() + 0.8f;
            AudioManager::getInstance()->playSoundEffect(SFX_HIT_02, 1.0f, pitch);
            _sprite->setRotation(0);
        }
        
        _collisionCount++;
    }
    
    if( (shapeA->getCollisionBitmask() == PLAYER_BITMASK && shapeB->getCollisionBitmask() == OBSTACLE_BITMASK) ||
        (shapeB->getCollisionBitmask() == PLAYER_BITMASK && shapeA->getCollisionBitmask() == OBSTACLE_BITMASK) )
    {
        
            if(!_immortal)
                removeHealth();
        
        
        _collisionCount++;
    }
    
    return true;
}

void Player::onContactSeparate(cocos2d::PhysicsContact &contact)
{
    auto shapeA = contact.getShapeA();
    auto shapeB = contact.getShapeB();
    
    if( (shapeA->getCollisionBitmask() == PLAYER_BITMASK && shapeB->getCollisionBitmask() == SOLID_BITMASK) ||
        (shapeB->getCollisionBitmask() == PLAYER_BITMASK && shapeA->getCollisionBitmask() == SOLID_BITMASK) )
    {
        _collisionCount--;
    }
    
}

void Player::setupAnimations()
{
    _idleAnimationFrames.push_back( "blocky_idle_01.png" );
    _idleAnimationFrames.push_back( "blocky_idle_02.png" );
    _idleAnimationFrames.push_back( "blocky_idle_03.png" );
    _idleAnimationFrames.push_back( "blocky_idle_04.png" );
    _idleAnimationFrames.push_back( "blocky_idle_05.png" );
    _idleAnimationFrames.push_back( "blocky_idle_06.png" );
    
    _dustParticleFrames.push_back(PARTICLE_DUST_01);
    _dustParticleFrames.push_back(PARTICLE_DUST_02);
    _dustParticleFrames.push_back(PARTICLE_DUST_03);
    _dustParticleFrames.push_back(PARTICLE_DUST_04);
    
}

void Player::resetAnimation()
{
    _animComponent->playAnimation(IDLE_ANIMATION_NAME, 12);
}

void Player::collisionEmit(cocos2d::Vec2 &pos, int xDir, int yDir )
{
    for(int i = 0; i < 12; i++)
    {
        float vx = 30 * CCRANDOM_0_1() + 10;
        float vyMultiplier = 1.0f * CCRANDOM_0_1() + 1.0f;
        float vy = 100 * CCRANDOM_0_1();
        float lifeTime = 0.2f * CCRANDOM_0_1() + 0.1f;
        float startScale = 0.4f * CCRANDOM_0_1() + 0.6f;
        float endScale = 0.4f * CCRANDOM_0_1();

        SimpleParticle* particle = SimpleParticle::create( PARTICLE_CIRCLE_01 , vx * xDir, vy , lifeTime, startScale, endScale);
        particle->setColor(PLAYER_COLOR);
        particle->setPosition(pos);
        _onLayer->addChild(particle);
    }
}

void Player::emitParticles(std::string _sprite, cocos2d::Color3B _color, cocos2d::Vec2& _pos, int _num, float _vx, float _vy, float _lifeTime,float _endScale)
{
    for(int i = 0; i < _num; i++)
    {
        float vx = _vx * CCRANDOM_0_1();
        float vy = _vy * CCRANDOM_0_1();
        
        SimpleParticle* particle = SimpleParticle::create(_sprite , vx, vy, _lifeTime,1.0f, _endScale);
        particle->setColor(_color);
        particle->setPosition(_pos);
        _onLayer->addChild(particle);
        
    }
}






