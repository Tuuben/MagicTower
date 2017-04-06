//
//  Food.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 23/02/16.
//
//

#ifndef Food_h
#define Food_h

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../Globals.h"
#include "Pickup.h"

class Food : public Pickup {
#define IDLE_MOVEMENT_SPEED 6.0f
public:
    static Food* create();
    bool init() override;
    
    void onPickUpObject() override;
    void destroySelf() override;
    void update(float dt) override;
    Food(){};
    ~Food(){};
    
private:
    cocos2d::Sprite* _sprite;
    float _elapsedTime;
    int _directionX = 1;
    int _directionY = 1;
    bool _canMove = true;

};

#endif /* Food_hpp */
