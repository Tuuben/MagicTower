//
//  Soul.c
//  MagicTowerProject
//
//  Created by Tobias Helsing on 13-04-2017.
//
//

#include "Soul.h"

bool Soul::init(){

    if(!Node::init()){
        return false;
    }
    
    auto d = DrawNode::create();
    d->drawSolidRect(Vec2(-4, -4), Vec2(4,4), Color4F::BLUE);
    addChild(d);
    
    return true;
    
}
