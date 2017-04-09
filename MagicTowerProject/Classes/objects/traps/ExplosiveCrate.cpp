//
//  ExplosiveCrate.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 09/04/2017.
//
//

#include "ExplosiveCrate.h"
#include "../../Globals.h"
#include "../../scenes/GameScene.h"
#include "../../candypunk/utility/Utils.h"
#include "../../candypunk/components/AnimationComponent.h"
#include "../SimpleParticle.h"

bool ExplosiveCrate::init(){
    
    if(!Node::init()){
        return false;
    }
    
    sprt = Sprite::createWithSpriteFrameName(EXPLOSIVE_CRATE_01);
    sprt->setColor(OBSTACLE_COLOR);
    addChild(sprt);

    player = GameScene::getInstance()->getPlayer();
    
    countDown = COUNTDOWN_VALUE;
    
    scheduleUpdate();
    
    return true;
    
}

void ExplosiveCrate::update(float dt){
    
    if(isDestroyed)
        return;
    
    Node::update(dt);

    if(!isTriggered)
        checkForPlayer();
    else
        doCountdown(dt);
    
}

void ExplosiveCrate::checkForPlayer(){

    
    if( getDistanceToPlayer() < MIN_TRIGGER_DISTANCE ){
        
        isTriggered = true;
        setFlashEffect();
        
    }

}

void ExplosiveCrate::doCountdown(float dt){
    
    countDown -= dt;
    
    if(countDown <= 0){
        explode();
    }
    
}

void ExplosiveCrate::explode(){
 
    GameScene::getInstance()->setScreenShake(0.2f, 24.0f, 24.0f);
    this->setVisible(false);
    isDestroyed = true;
    
    // Effect
    for(int i = 0; i < 6; i++){
    
        float angle = (6 / (float)i) * MATH_PIOVER2 * 4;
        float lifetime = 0.6f * CCRANDOM_0_1() + 0.2f;
        auto part = SimpleParticle::create(PARTICLE_EXPLOSION_01, cosf(angle) * 20.0f, sinf(angle) * 20.0f, lifetime, 2.0f, 2.0f);
        part->setPosition( Vec2( getPositionX() + ( (100 * CCRANDOM_0_1()) - 50 ), getPositionY() + ( (100 * CCRANDOM_0_1()) - 50 ) ) );
        part->setIgnoreGravity(true);
        part->setGlobalZOrder(100);
        part->setRotation(180 * CCRANDOM_0_1());
        part->setColor( (i % 2 == 1) ? Color3B(223, 223, 223) : Color3B(255, 255, 255));
        auto anim = AnimationComponent::createComponent(part);
        anim->addAnimation("boom", {PARTICLE_EXPLOSION_01, PARTICLE_EXPLOSION_02, PARTICLE_EXPLOSION_03, PARTICLE_EXPLOSION_04});
        anim->playAnimation("boom", 8, false);
        part->addComponent(anim);
        this->getParent()->addChild(part);
    
    }
    
    for(int i = 0; i < 18; i++){
        
        float size = 1.6f * CCRANDOM_0_1() + 0.4f;
        float lifetime = 1.8f * CCRANDOM_0_1() + 0.6f;
        float vx = 300.0f * CCRANDOM_0_1() - 150.0f;
        float vy = 300.0f * CCRANDOM_0_1() - 0.0f;
        auto part = SimpleParticle::create(PARTICLE_CIRCLE_01, vx, vy, lifetime, size, 0.0f);
        part->setIgnoreGravity(false);
        part->setPosition(getPosition());
        part->setGlobalZOrder(100);
        part->setColor( (i % 2 == 1) ? Color3B(223, 223, 223) : Color3B(255, 255, 255));
        this->getParent()->addChild(part);
        
    }
    
}

void ExplosiveCrate::setFlashEffect(){

    auto setWhite = CallFunc::create( [this](){ this->sprt->setColor(Color3B::WHITE); } );
    auto setRed = CallFunc::create( [this](){ this->sprt->setColor(OBSTACLE_COLOR); } );
    auto wait = DelayTime::create(0.1f);
    auto seq = Sequence::create(setWhite, wait, setRed, wait, NULL);
    this->runAction(RepeatForever::create(seq));
    
}

float ExplosiveCrate::getDistanceToPlayer(){

    if(!player)
        return 0.0f;
    
    Vec2 playerPos = player->getParent()->convertToWorldSpace( player->getPosition() );
    Vec2 curPos = getParent()->convertToWorldSpace(getPosition());
    return Utils::distance(curPos, playerPos);
    
}
