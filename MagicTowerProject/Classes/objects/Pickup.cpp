//
//  Pickup.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 27-02-2016.
//
//

#include "Pickup.h"
#include "../scenes/GameScene.h"
#include "../candypunk/utility/Utils.h"

using namespace cocos2d;

Pickup* Pickup::create(cocos2d::Size physicsBodySize, int bitmask, int colidingBitmask)
{
    Pickup* _pickup = new Pickup();
    
    if(_pickup && _pickup->init(physicsBodySize, bitmask, colidingBitmask))
    {
        _pickup->autorelease();
        return _pickup;
    }
    
    CC_SAFE_DELETE(_pickup);
    return NULL;
}

bool Pickup::init(cocos2d::Size physicsBodySize, int bitmask, int colidingBitmask)
{
    
    _colideBitmask = colidingBitmask;
    
    _animComponent = AnimationComponent::createComponent(this);
    this->addComponent(_animComponent);
    
    auto pBody = PhysicsBody::createEdgeBox( physicsBodySize, PhysicsMaterial(0,0,0) );
    pBody->setCollisionBitmask(bitmask);
    pBody->setContactTestBitmask(true);
    pBody->setDynamic(false);
    pBody->getShape(0)->setSensor(true);
    pBody->setPositionOffset(Vec2( physicsBodySize.width / 2, physicsBodySize.height /2 ));
    this->setPhysicsBody(pBody);
    
    
    auto onContactEventListener = EventListenerPhysicsContact::create();
    onContactEventListener->onContactBegin = CC_CALLBACK_1( Pickup::onContactBegin, this );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(onContactEventListener, this);
    
    return true;
}

AnimationComponent* Pickup::getAnimationComponent(){ return _animComponent; }

bool Pickup::onContactBegin(cocos2d::PhysicsContact &contact)
{
    auto shapeA = contact.getShapeA();
    auto shapeB = contact.getShapeB();
    
    if( (shapeA->getCollisionBitmask() == _colideBitmask && shapeB->getBody()->getNode() == this) ||
        (shapeB->getCollisionBitmask() == _colideBitmask && shapeA->getBody()->getNode() == this) )
    {
        onPickUpObject();
        this->getPhysicsBody()->setEnabled(false);
        
        return false;
    }
    
    return true;
}

void Pickup::moveTowardPlayer(float speed, float minDistance, float dt){

    if(GameScene::getInstance() != nullptr){
        
        auto playerObj = GameScene::getInstance()->getPlayer();
        Vec2 playerPos = playerObj->getParent()->convertToWorldSpace( playerObj->getPosition() );
        Vec2 curPos = getParent()->convertToWorldSpace(getPosition());
        
        Vec2 diff = Vec2( (curPos.x - playerPos.x) * (curPos.x - playerPos.x), ( curPos.y - playerPos.y) * (curPos.y - playerPos.y) );
        float distance = sqrtf(diff.x - diff.y);
        float angle = atan2( playerPos.y - curPos.y, playerPos.x - curPos.x);
        
        
        
        if( (distance < minDistance) || _foundPlayer){
            Vec2 newPos = Utils::lerp(curPos, playerPos, dt * speed );
            
            Vec2 v = Vec2( cos(angle) * speed * dt, sin(-angle) * speed * dt );
            setPosition( getPosition() + v );
            _foundPlayer = true;
        }
        
    }
  
}

void Pickup::onPickUpObject()
{
    CCLOG("runs pickup");
}

void Pickup::destroySelf()
{
    this->setVisible(false);
}

