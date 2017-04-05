//
//  ClutterTile.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 17/02/16.
//
//

#ifndef ClutterTile_h
#define ClutterTile_h

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../Globals.h"
#include "../candypunk/components/AnimationComponent.h"

enum ClutterType
{
    GRASS = 0,
    BACKGROUND,
};

class ClutterTile : public cocos2d::Sprite {
    
public:
    static ClutterTile* create( ClutterType clutterType );
    bool init( ClutterType clutterType );
    
private:
    void update(float dt);
    ClutterTile(){}
    ~ClutterTile(){}

private:
    ClutterType _clutterType;
    AnimationComponent* _animComp;
    
};

#endif /* ClutterTile_h */
