//
//  Actor.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 11/04/2017.
//
//

#ifndef Actor_hpp
#define Actor_hpp

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"
USING_NS_CC;

class Actor : public Node{

public:
    CREATE_FUNC(Actor);
    bool init();
    
    virtual void freeze();
    virtual void unfreeze();
    
    bool isFrozen(){ return frozen; };
    void update(float dt) override;
    
    
private:
    bool frozen = false;
    
};


#endif /* Actor_hpp */
