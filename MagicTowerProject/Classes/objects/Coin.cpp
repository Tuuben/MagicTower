//
//  Coin.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 26/02/16.
//
//

#include "Coin.h"
#include "../handlers/AudioManager.h"
#include "../objects/SimpleParticle.h"

using namespace cocos2d;

Coin* Coin::create()
{
    Coin* _coin = new Coin();
    
    if(_coin && _coin->init())
    {
        _coin->autorelease();
        return _coin;
    }
    
    CC_SAFE_DELETE(_coin);
    return NULL;
}

bool Coin::init()
{
    Pickup::init(Size(16, 16), COIN_BITMASK, PLAYER_BITMASK);
    
    initWithSpriteFrameName(COIN_IDLE_01);
    
    setColor(Color3B(248, 184, 0));
    

    getAnimationComponent()->addAnimation("idle", { COIN_IDLE_01, COIN_IDLE_02, COIN_IDLE_03, COIN_IDLE_04, COIN_IDLE_05, COIN_IDLE_06 });
    getAnimationComponent()->addAnimation("pick-up", { COIN_PICK_UP_01, COIN_PICK_UP_02, COIN_PICK_UP_03, COIN_PICK_UP_04, COIN_PICK_UP_05, COIN_PICK_UP_06, COIN_PICK_UP_07 });
    
    getAnimationComponent()->playAnimation("idle", 12, true, true);
    
    return true;
}

void Coin::onPickUpObject()
{
    getAnimationComponent()->playAnimation("pick-up", 16, false);
    getAnimationComponent()->onAnimationFinished( CC_CALLBACK_0(Coin::destroySelf, this) );
    
    this->getPhysicsBody()->setEnabled(false);
    
    AudioManager::getInstance()->playSoundEffect( SFX_PICK_UP_05, 0.6f, 1.0f);
}

void Coin::destroySelf()
{
    Pickup::destroySelf();
}

