//
//  Pickup.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 27-02-2016.
//
//

#ifndef Pickup_hpp
#define Pickup_hpp

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../candypunk/components/AnimationComponent.h"

class Pickup : public cocos2d::Sprite {
    
public:
    static Pickup* create(cocos2d::Size physicsBodySize, int bitmask, int colidingBitmask);
    virtual bool init(cocos2d::Size physicsBodySize, int bitmask, int colidingBitmask);
    AnimationComponent* getAnimationComponent();
    virtual void moveTowardPlayer(float speed, float distance, float dt);
    virtual void onPickUpObject();
    virtual void destroySelf();
    bool hasFoundPlayer(){return _foundPlayer;};
    
    Pickup(){}
    ~Pickup(){}
    
private:
    bool onContactBegin(cocos2d::PhysicsContact &contact);
    
private:
    AnimationComponent* _animComponent;
    int _colideBitmask = 0;
    bool _foundPlayer = false;
};

#endif /* Pickup_hpp */
