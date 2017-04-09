//
//  SimpleParticle.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 14-02-2016.
//
//

#include "SimpleParticle.h"

using namespace cocos2d;

SimpleParticle* SimpleParticle::create(std::string _sprName, float vx, float vy, float lifeTime,float startScale, float endScale)
{
    SimpleParticle* _sp = new SimpleParticle();
    
    if(_sp && _sp->init(_sprName, vx, vy, lifeTime,startScale, endScale))
    {
        _sp->autorelease();
        return _sp;
    }

    CC_SAFE_DELETE(_sp);
    return NULL;
}

bool SimpleParticle::init(std::string _sprName, float vx, float vy, float lifeTime,float startScale, float endScale)
{
    initWithSpriteFrameName(_sprName);
    
    _vx = vx;
    _vy = vy;
    _maxLifeTime = lifeTime;
    _lifeTime = lifeTime;
    _startScale = startScale;
    _endScale = endScale;
    
    this->scheduleUpdate();
    
    return true;
}

void SimpleParticle::update(float dt)
{
    Sprite::update(dt);
    
    if(_dead) return;
    
    _lifeTime -= dt;

    float scalePercentage = _lifeTime / _maxLifeTime;
    
    this->setScale( _endScale + (_startScale - _endScale) * scalePercentage );
    
    this->setPosition( cocos2d::Vec2( this->getPositionX() + (_vx * dt), this->getPositionY() + (_vy * dt) ) );
    
    if(!_ignoreGravity){
        
        _vy -= _gravity;
        
    }
    
    if(_lifeTime <= 0 && !_dead)
    {
        this->removeFromParent();
        _dead = true;
    }
}

void SimpleParticle::setIgnoreGravity(bool enabled){ _ignoreGravity = enabled; }

