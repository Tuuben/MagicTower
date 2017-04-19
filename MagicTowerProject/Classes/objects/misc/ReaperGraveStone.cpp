//
//  GraveStone.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 19/04/2017.
//
//

#include "ReaperGraveStone.h"
#include "../../Globals.h"

bool ReaperGraveStone::init(){

    if(!Node::init()){
        return false;
    }
    
    sprt = Sprite::createWithSpriteFrameName(GRIM_REAPER_GRAVESTONE_01);
    sprt->setGlobalZOrder(-100);
    addChild(sprt);
    
    return true;
    
}
