//
//  Swing.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 08/04/2017.
//
//

#ifndef Swing_hpp
#define Swing_hpp

#include <stdio.h>
#include "../../candypunk/objects/Actor.h"

USING_NS_CC;

class Swing : public Actor {
#define SPEED_MULTIPLIER 0.3f
#define CHAIN_SPACING 16.0f
public:
    CREATE_FUNC(Swing);
    bool init();
private:
    std::vector<Node*> chainNodes;
    float angle = 0.0f;
    int direction = 1;
    
    void createChainNodes(int amt);
    void update(float dt);
};

#endif /* Swing_hpp */
