//
//  SimpleParticle.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 14-02-2016.
//
//

#ifndef SimpleParticle_h
#define SimpleParticle_h

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"

class SimpleParticle : public cocos2d::Sprite
{
public:
    static SimpleParticle* create(std::string _sprName, float vx, float vy, float lifeTime,float startScale, float endScale);
    bool init(std::string _sprName,float vx, float vy, float lifeTime,float startScale, float endScale);
    void update(float dt);
    
    void setGravityEnabled(bool enabled);
private:
    float _gravity = 9.8f;
    float _vx;
    float _vy;
    float _maxLifeTime = 0;
    float _lifeTime = 0;
    float _startScale = 0;
    float _endScale = 0;
    bool _dead = false;
    bool _ignoreGravity = false;
    
};

#endif /* SimpleParticle_h */
