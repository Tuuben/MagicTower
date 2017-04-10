//
//  Background.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 10/04/2017.
//
//

#ifndef Background_hpp
#define Background_hpp

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"
USING_NS_CC;

class Background : public Sprite {

public:
    CREATE_FUNC(Background);
    bool init();
    float getTopPos();
    void setOther(Background* bg){ otherBg = bg; };
    void update(float dt) override;
    
private:
    Background* otherBg = nullptr;
};

#endif /* Background_hpp */
