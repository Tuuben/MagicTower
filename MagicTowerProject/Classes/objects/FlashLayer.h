//
//  FlashLayer.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 24/02/16.
//
//

#ifndef FlashLayer_h
#define FlashLayer_h

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"

class FlashLayer : public cocos2d::LayerColor {

    
public:
    static FlashLayer* create(cocos2d::Color3B color);
    bool init(cocos2d::Color3B color);
    void flash( int startOpacity, float time);
private:
    float _lifeTime = 0;
    FlashLayer(){}
    ~FlashLayer(){}
};

#endif /* FlashLayer_hpp */
