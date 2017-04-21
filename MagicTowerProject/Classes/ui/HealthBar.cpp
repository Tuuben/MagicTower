//
//  HealthBar.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 21/04/2017.
//
//

#include "HealthBar.h"
#include "../PlayerStats.h"
#include "../Globals.h"
#include "../objects/SimpleParticle.h"

bool HealthBar::init(){

    if(!Node::init()){
        return  false;
    }
    
    createHealthBar();
    
    return true;
}

void HealthBar::removeHeart(){

    if(heartList.size() > 0){
        
        int lastIndex = heartList.size() - 1;
        auto h = heartList.at(lastIndex);
        auto white = CallFunc::create( [h]() mutable { h->setColor(Color3B::WHITE); } );
        auto red = CallFunc::create( [h]() mutable { h->setColor(Color3B::RED); } );
        auto wait = DelayTime::create(0.1f);
        auto seq = Sequence::create(white, wait, red, wait, NULL);
        auto onComplete = CallFunc::create( [this, h, lastIndex]() mutable {

            for(int i = 0; i < 12; i++){
                float vx = 30.0f * CCRANDOM_0_1() - 15.0f;
                float vy = 40.0f * CCRANDOM_0_1() + 20.0f;
                auto part = SimpleParticle::create(PARTICLE_SQUARE_01, vx, vy, 0.5, 1.0f, 0);
                part->setPosition(h->getPosition());
                part->setColor(Color3B::RED);
                h->getParent()->addChild(part);
            }
            
            h->removeFromParent();
            this->heartList.erase(heartList.begin() + lastIndex);
            
        } );
        
        runAction( Sequence::create(Repeat::create(seq, 3), onComplete, NULL) );
        
    }
    
}

void HealthBar::createHealthBar(){

    int numLives = PlayerStats::getInstance()->getLives();
    for(int i = 0; i < numLives; i++){
        
        auto h = Sprite::createWithSpriteFrameName(HEART_01);
        h->setPositionX(10 * i);
        h->setColor(Color3B(255,0,0));
        addChild(h);
        heartList.push_back(h);
    }

    runAction(CallFunc::create([this](){ this->alignBar(); }));
    
}

void HealthBar::alignBar(){

    float barWidth = 10 * (PlayerStats::getInstance()->getLives() - 1);
    this->setPositionX( this->getPositionX() - (barWidth / 2) );
    
}
