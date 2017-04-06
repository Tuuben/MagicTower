//
//  Food.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 23/02/16.
//
//

#include "Food.h"
#include "../handlers/AudioManager.h"

using namespace cocos2d;

Food* Food::create()
{
    Food* _food = new Food();
    
    if(_food && _food->init())
    {
        _food->autorelease();
        return _food;
    }
    
    CC_SAFE_DELETE(_food);
    return NULL;
}

bool Food::init()
{
    Pickup::init(Size(16, 16), FOOD_BITMASK, PLAYER_BITMASK);
    
    initWithSpriteFrameName(FOOD_IDLE_01);
    setColor( FOOD_COLOR );
    
    getAnimationComponent()->addAnimation("normal", { FOOD_IDLE_01, FOOD_IDLE_02, FOOD_IDLE_03, FOOD_IDLE_04 } );
    getAnimationComponent()->addAnimation("pick-up", { FOOD_PICK_UP_01, FOOD_PICK_UP_02, FOOD_PICK_UP_03, FOOD_PICK_UP_04,FOOD_PICK_UP_05,FOOD_PICK_UP_06,FOOD_PICK_UP_07 } );
    getAnimationComponent()->playAnimation("normal", 8, true, true);
    
    this->scheduleUpdate();
    
    _directionX = (CCRANDOM_0_1() < 0.5f) ? -1 : 1;
    _directionY = (CCRANDOM_0_1() < 0.5f) ? -1 : 1;
    
    return true;
}

void Food::onPickUpObject()
{
    getAnimationComponent()->playAnimation("pick-up", 16, false);
    getAnimationComponent()->onAnimationFinished( CC_CALLBACK_0(Food::destroySelf, this) );
    
    this->getPhysicsBody()->setEnabled(false);
    
    AudioManager::getInstance()->playSoundEffect( SFX_PICK_UP_03, 0.6f, 1.0f);
    
    _canMove = false;
    
}

void Food::destroySelf()
{
    Pickup::destroySelf();
}

void Food::update(float dt){

    Pickup::update(dt);
    
    if(!_canMove)
        return;
    
    Vec2 v = Vec2( cos(_elapsedTime) * IDLE_MOVEMENT_SPEED * _directionX * dt, sin(_elapsedTime) * IDLE_MOVEMENT_SPEED * _directionY * dt);
    setPosition( getPosition() + v);
    
    moveTowardPlayer(20.0f, 10.0f, dt);
    
    _elapsedTime += dt;
    
}
