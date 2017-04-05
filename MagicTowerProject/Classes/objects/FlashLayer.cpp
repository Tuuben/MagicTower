//
//  FlashLayer.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 24/02/16.
//
//

#include "FlashLayer.h"

using namespace cocos2d;

FlashLayer* FlashLayer::create(cocos2d::Color3B color)
{
    FlashLayer* _flashLayer = new FlashLayer();
    
    if(_flashLayer && _flashLayer->init(color))
    {
        _flashLayer->autorelease();
        return _flashLayer;
    }
    
    CC_SAFE_DELETE(_flashLayer);
    
    return NULL;
}

bool FlashLayer::init(cocos2d::Color3B color)
{
    initWithColor(cocos2d::Color4B::BLACK);
    
    this->setOpacity(0);
    
    this->setColor(color);

    return true;
}

void FlashLayer::flash(int startOpacity, float time)
{
    this->setOpacity(startOpacity);
    
    auto fadeTo = FadeTo::create(time, 0);
    
    this->runAction(fadeTo);
}