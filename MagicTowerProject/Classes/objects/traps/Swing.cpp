//
//  Swing.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 08/04/2017.
//
//

#include "Swing.h"
#include "../../Globals.h"

bool Swing::init(){

    if(!Node::init()){
        return false;
    }
    
    auto sprite = Sprite::createWithSpriteFrameName(SWING_BASE_01);
    sprite->setColor(OBSTACLE_COLOR);
    sprite->setGlobalZOrder(50);
    addChild(sprite);
    
    auto pbody = PhysicsBody::createBox(Size( 16,16 ));
    pbody->setDynamic(false);
    addComponent(pbody);
    
    direction = (CCRANDOM_0_1() < 0.5) ? -1 : 1;
    
    createChainNodes(4);
    
    scheduleUpdate();
    
    return true;
    
}

void Swing::createChainNodes(int amt){

    for(int i = 0; i < amt; i++){
        
        auto sprite = Sprite::createWithSpriteFrameName(SWING_CHAIN_01);
        sprite->setColor(OBSTACLE_COLOR);
        sprite->setGlobalZOrder(60);
        auto p = PhysicsBody::createBox(Size(8,8));
        p->setDynamic(true);
        p->setGravityEnable(false);
        p->setRotationEnable(false);
        p->getShape(0)->setSensor(true);
        sprite->addComponent(p);
        addChild(sprite);
        chainNodes.push_back(sprite);
        
    }
    
    //End Object
    auto sprite = Sprite::createWithSpriteFrameName(SWING_MACE_01);
    sprite->setColor(OBSTACLE_COLOR);
    sprite->setGlobalZOrder(60);
    auto p = PhysicsBody::createBox(Size(16,16));
    p->setDynamic(true);
    p->setGravityEnable(false);
    p->setRotationEnable(false);
    p->getShape(0)->setSensor(true);
    sprite->addComponent(p);
    addChild(sprite);
    chainNodes.push_back(sprite);

    
}

void Swing::update(float dt){

    if(isFrozen())
        return;
    
    for(int i = 0; i < chainNodes.size(); i++){
        
        angle += dt * SPEED_MULTIPLIER * direction;
        int index = i + 1;
        chainNodes[i]->setPosition( Vec2( cos(angle) * index * CHAIN_SPACING, sin(angle) * index * CHAIN_SPACING ) );
    
    }

}


