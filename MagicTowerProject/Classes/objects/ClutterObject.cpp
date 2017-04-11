//
//  ClutterObject.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 10/04/2017.
//
//

#include "ClutterObject.h"
#include "../Globals.h"
#include "SimpleParticle.h"
#include "GameScene.h"

bool ClutterObject::init(){

    if(!Node::init()){
        return false;
    }
    
    // Create sprite
    std::string sprNames[] = { VASE_SPRITE_01, CANDLE_STICK_SPRITE_01, CRATE_SPRITE_01 };
    int rand = 3 * CCRANDOM_0_1();
    
    if(rand == 1){
        sprt = Sprite::createWithSpriteFrameName(VASE_SPRITE_01);
        sprtColor = Color3B(152,120,248);
    }
    else if(rand == 2){
        sprt = Sprite::createWithSpriteFrameName(CANDLE_STICK_SPRITE_01);
        sprtColor = Color3B(255,205,10);
    }
    else {
        sprt = Sprite::createWithSpriteFrameName(CRATE_SPRITE_01);
        sprtColor = Color3B(226,82,25);
    }
    
    sprt->setColor(sprtColor);
    
    addChild(sprt);
    
    // Create Physics body
    auto pBody = PhysicsBody::createBox(Size(16,16));
    pBody->setContactTestBitmask(true);
    pBody->setCollisionBitmask(CLUTTER_OBJECT_BITMASK);
    pBody->setGravityEnable(false);
    pBody->setDynamic(false);
    addComponent(pBody);

    // Setup events
    setupCollisionEvents();
    
    return true;

}

void ClutterObject::setupCollisionEvents(){

    auto contactEventListener = EventListenerPhysicsContact::create();
    contactEventListener->onContactBegin = CC_CALLBACK_1( ClutterObject::onContactBegin, this );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactEventListener, this);
    
}

bool ClutterObject::onContactBegin(PhysicsContact &c){

    auto sA = c.getShapeA();
    auto sB = c.getShapeB();
    
    if( (sA->getBody()->getNode() == this && sB->getCollisionBitmask() == PLAYER_BITMASK) ||
        (sB->getBody()->getNode() == this && sA->getCollisionBitmask() == PLAYER_BITMASK) ) {
            breakObject();
       }
    
    return true;
    
}

void ClutterObject::breakObject(){

    getPhysicsBody()->setEnabled(false);
    this->setVisible(false);
    
    GameScene::getInstance()->setScreenShake(0.1, 4.0f, 4.0f);
    
    for(int i = 0; i < 8; i++){
    
        std::string strNames[] = { PARTICLE_CLUTTER_01, PARTICLE_CLUTTER_02, PARTICLE_CLUTTER_03 };
        float vx = 240.0f * CCRANDOM_0_1() - 120.0f;
        float vy = 120.0f * CCRANDOM_0_1() + 40.0f;
        float lifeTime = 0.2f * CCRANDOM_0_1() + 0.2f;
        float size = 2.0f * CCRANDOM_0_1() + 1.0f;
        //strNames[(int)(3 * CCRANDOM_0_1())]
        auto part = SimpleParticle::create( PARTICLE_SQUARE_01, vx, vy, lifeTime, size, 0.0f);
        part->setPosition(getPosition());
        part->setColor(sprtColor);
        part->setGlobalZOrder(50);
//        part->setRotation(360.0f * CCRANDOM_0_1());
        this->getParent()->addChild(part);
        
    }
    
    for( int i = 0; i < 4; i++ ){
    
        float vx = 20.0f * CCRANDOM_0_1() - 10.0f;
        float vy = 20.0f * CCRANDOM_0_1() - 10.0f;
        float lifeTime = 0.4f * CCRANDOM_0_1() + 0.2f;
        float startScale = 1.0f * CCRANDOM_0_1() + 0.5f;
        auto part = SimpleParticle::create( PARTICLE_CIRCLE_01 , vx, vy, lifeTime, startScale, startScale);
        float xOffset = 16.0f * CCRANDOM_0_1() - 8.0f;
        float yOffset = 16.0f * CCRANDOM_0_1() - 8.0f;
        part->setPosition( Vec2( getPositionX() + xOffset, getPositionY() + yOffset ) );
        part->setColor( (i % 2 == 1) ? Color3B(223, 223, 223) : Color3B(255, 255, 255));
        part->setIgnoreGravity(true);
        part->setGlobalZOrder(50);
        this->getParent()->addChild(part);
        
    }
    
}






