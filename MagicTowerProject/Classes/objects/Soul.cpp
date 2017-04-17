//
//  Soul.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 17-04-2017.
//
//

#include "Soul.h"
#include "../scenes/GameScene.h"

bool Soul::init(){

    if(!Actor::init())
        return false;

    auto dNOde = DrawNode::create();
    dNOde->drawSolidRect(Vec2(-4, -4), Vec2(4,4), Color4F::YELLOW);
    addChild(dNOde);
    
    scheduleUpdate();
    
    return true;
}

void Soul::update(float dt){

    if(isFrozen())
        return;
    
    Actor::update(dt);
    
    if(player != nullptr)
        moveTowardPlayer(dt);
    else
        findPlayer();
    
}

void Soul::moveTowardPlayer(float dt){
    
    elapsed += dt;
    
    Vec2 curPos = getPosition();
    Vec2 curPlayerPos = Vec2(player->getPositionX(), player->getPositionY() - 10.0f);
    float distance = sqrtf( powf(curPlayerPos.x - curPos.x, 2) + powf(curPlayerPos.y - curPos.y, 2.0f) );
    float angle = atan2f(curPlayerPos.y - curPos.y, curPlayerPos.x - curPos.x);
    
    Vec2 vel = Vec2( cosf(angle) * MOVEMENT_SPEED * dt, sinf(angle) * MOVEMENT_SPEED * dt );
    
    setPosition(getPosition() + vel);
    
    
}

void Soul::findPlayer(){
    
    player = GameScene::getInstance()->getPlayer();
    
}
