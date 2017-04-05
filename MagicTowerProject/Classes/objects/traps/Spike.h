//
//  Spike.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 27-02-2016.
//
//

#ifndef Spike_h
#define Spike_h

#include <stdio.h>
#include "../../../cocos2d/cocos/cocos2d.h"

class Spike : public cocos2d::Sprite {
    
public:
    enum DIRECTION
    {
        TOP = 0,
        RIGHT,
        BOTTOM,
        LEFT
    };
    
    static Spike* create();
    bool init();
    void setDirection(DIRECTION dir);
};

#endif /* Spike_h */
