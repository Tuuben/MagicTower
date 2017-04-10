//
//  Background.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 10/04/2017.
//
//

#include "Background.h"
#include "../scenes/GameScene.h"

bool Background::init(){

    if(!Sprite::initWithFile("textures/background.png")){
        return false;
    }
    
    setAnchorPoint(Vec2::ZERO);
    setGlobalZOrder(-100);

    scheduleUpdate();
    
    return true;
}

float Background::getTopPos(){

    return getPositionY() + getContentSize().height;
    
}

void Background::update(float dt){

    Sprite::update(dt);
    
    if(otherBg == nullptr){
        return;
    }
    
    if(getTopPos() < GameScene::getInstance()->getCameraPositionY()){
    
        setPosition( Vec2(0, otherBg->getTopPos()) );
        
    }
    
}
