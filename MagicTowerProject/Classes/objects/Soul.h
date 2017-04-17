//
//  Soul.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 17-04-2017.
//
//

#ifndef Soul_hpp
#define Soul_hpp

#include <stdio.h>
#include "../candypunk/objects/Actor.h"
#include "../candypunk/components/AnimationComponent.h"
#include "Player.h"

USING_NS_CC;

class Soul : public Actor{
#define MOVEMENT_SPEED 80.0f
public:
    CREATE_FUNC(Soul);
    bool init();
    void update(float dt) override;
    void moveTowardPlayer(float dt);
    void findPlayer();
    
    Player* player = nullptr;
    float elapsed = 0.0f;
};

#endif /* Soul_hpp */
