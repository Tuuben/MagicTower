//
//  ExplosiveCrate.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 09/04/2017.
//
//

#ifndef ExplosiveCrate_hpp
#define ExplosiveCrate_hpp

#include <stdio.h>
#include "../../candypunk/objects/Actor.h"
#include "../Player.h"

USING_NS_CC;
class ExplosiveCrate : public Actor {
#define MIN_TRIGGER_DISTANCE 30
#define COUNTDOWN_VALUE 1.4f
public:
    CREATE_FUNC(ExplosiveCrate);
    bool init();
private:
    Sprite* sprt;
    Player* player = nullptr;
    
    float countDown = 3.0f;
    bool isTriggered = false;
    bool isDestroyed = false;
    void update(float dt) override;
    void checkForPlayer();
    void doCountdown(float dt);
    void explode();
    void setFlashEffect();
    float getDistanceToPlayer();

};

#endif /* ExplosiveCrate_hpp */
