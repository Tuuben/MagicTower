//
//  ClutterObject.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 10/04/2017.
//
//

#ifndef ClutterObject_hpp
#define ClutterObject_hpp

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"
USING_NS_CC;

class ClutterObject : public Node {
    
public:
    CREATE_FUNC(ClutterObject);
    bool init();
    
private:
    Sprite* sprt;
    Color3B sprtColor;
    
    void setupCollisionEvents();
    bool onContactBegin(PhysicsContact& c);
    void breakObject();
    
};

#endif /* ClutterObject_hpp */
