//
//  Bat.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 09/04/2017.
//
//

#include "Bat.h"
#include "../../Globals.h"
#include "../../scenes/GameScene.h"

bool Bat::init(){
    
    if(!Node::init()){
        return false;
    }
    
    sprt = Sprite::createWithSpriteFrameName(BAT_01);
    sprt->setColor(OBSTACLE_COLOR);
    sprt->setGlobalZOrder(80);
    addChild(sprt);
    
    auto pbody = PhysicsBody::createCircle(4);
    pbody->setGravityEnable(false);
    pbody->setDynamic(false);
    pbody->setCollisionBitmask(OBSTACLE_BITMASK);
    pbody->setContactTestBitmask(true);
    pbody->setRotationEnable(false);
    pbody->getShape(0)->setSensor(true);
    addComponent(pbody);
    
    animComp = AnimationComponent::createComponent(sprt);
    animComp->addAnimation("idle", {BAT_01, BAT_02, BAT_03, BAT_04});
    animComp->playAnimation("idle", 12, true, true);
    addComponent(animComp);
    
    player = GameScene::getInstance()->getPlayer();
    
    return true;

}

void Bat::update(float dt){

    Node::update(dt);
    
    moveTowardPlayer(dt);
    
}

void Bat::freeze(){
    
    Actor::freeze();
    animComp->stopAnimation();
    
}

void Bat::unfreeze(){

    Actor::unfreeze();
    animComp->playAnimation("idle", 12, true, true);
    
}

void Bat::moveTowardPlayer(float dt){
    
    if(player == nullptr){
        return;
    }

    Vec2 playerPos = player->getParent()->convertToWorldSpace( player->getPosition() );
    Vec2 curPos = getParent()->convertToWorldSpace(getPosition());
    
    // Get the distance to player
    Vec2 diff = Vec2( powf((curPos.x - playerPos.x), 2), powf(( curPos.y - playerPos.y), 2) );
    float distance = sqrtf(diff.x + diff.y);
    
    // Get angle to player
    float angle = atan2( playerPos.y - curPos.y, playerPos.x - curPos.x);
    
    if(distance < MIN_DISTANCE_TO_PLAYER){
    
        Vec2 v = Vec2( cosf(angle) * SPEED * dt, sinf(angle) * SPEED * dt );
        setPosition( getPosition() + v );
        
    }
    
}




