//
//  Spike.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 27-02-2016.
//
//

#include "Spike.h"
#include "../../Globals.h"
using namespace cocos2d;
Spike* Spike::create()
{
    Spike* _spike = new Spike();
    
    if(_spike && _spike->init())
    {
        _spike->autorelease();
        return _spike;
    }
    
    CC_SAFE_DELETE(_spike);
    return NULL;
}

bool Spike::init()
{
    initWithSpriteFrameName(SPIKE_SPRITE_01);
    
    setColor( OBSTACLE_COLOR );
    
    auto pbod = PhysicsBody::createBox(Size( 8, 2 ) );
    pbod->setContactTestBitmask(true);
    pbod->setCollisionBitmask(OBSTACLE_BITMASK);
    pbod->setDynamic(false);
    this->setPhysicsBody(pbod);
    
    return true;
}

void Spike::setDirection(DIRECTION dir)
{
    switch (dir) {
            
        case DIRECTION::TOP:
            
            break;
            
        case DIRECTION::RIGHT:
            this->setRotation(90);
            break;
            
        case DIRECTION::BOTTOM:
            this->setScale(1, -1);
            break;
            
        case DIRECTION::LEFT:
            this->setRotation(-90);
            break;
            
        default:
            break;
    }
    
}