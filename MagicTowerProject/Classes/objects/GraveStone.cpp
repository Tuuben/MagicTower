//
//  GraveStone.c
//  MagicTowerProject
//
//  Created by Tobias Helsing on 13-04-2017.
//
//

#include "GraveStone.h"
#include "../Globals.h"

bool GraveStone::init(){

    if(!Node::init()){
        return false;
    }
    
    auto dbug = DrawNode::create();
    dbug->drawSolidRect(Vec2(-8, -8), Vec2(8,8), Color4F::RED);
    addChild(dbug);
    
    auto pBody = PhysicsBody::createBox(Size(16,16));
    pBody->setDynamic(false);
    pBody->setCollisionBitmask(GRAVE_OBJECT_BITMASK);
    pBody->setContactTestBitmask(true);
    addComponent(pBody);
    
    return true;
    
}
