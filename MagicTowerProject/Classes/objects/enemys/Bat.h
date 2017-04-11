//
//  Bat.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 09/04/2017.
//
//

#ifndef Bat_hpp
#define Bat_hpp

#include <stdio.h>
#include "../../candypunk/objects/Actor.h"
#include "../../candypunk/components/AnimationComponent.h"

USING_NS_CC;

class Bat : public Actor{
#define MIN_DISTANCE_TO_PLAYER 280.0f
#define SPEED 30.0f
public:
    CREATE_FUNC(Bat);
    bool init() override;
    void freeze() override;
    void unfreeze() override;
    
private:
    Sprite* sprt;
    AnimationComponent* animComp;
    bool foundPlayer;
    Node* player = nullptr;
    
    void update(float dt) override;
    void moveTowardPlayer(float dt);

    
};

#endif /* Bat_hpp */
