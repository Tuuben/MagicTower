//
//  AnimationComponent.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 18/02/16.
//
//

#include "AnimationComponent.h"

using namespace cocos2d;

AnimationComponent* AnimationComponent::createComponent(cocos2d::Sprite* animateSprite)
{
    AnimationComponent* _animComp = new AnimationComponent();
    
    if(_animComp && _animComp->init(animateSprite))
    {
        _animComp->autorelease();
        return _animComp;
    }
    
    CC_SAFE_DELETE(_animComp);
    return NULL;
}

bool AnimationComponent::init(cocos2d::Sprite* animateSprite)
{
    Component::init();
    
    _animSprite = animateSprite;
    
    this->setName("AnimationComponent");
    
    return true;
}

void AnimationComponent::update(float dt)
{
//    CCLOG("===================");
//    CCLOG("delta time %f", dt);
//    CCLOG("time elapsed %f", _timeElapsed);
//    CCLOG("frame index %d", _frameIndex);
//    CCLOG("frame rate %d", _frameRate);
//    CCLOG("cur animation name %s", _curAnimationName.c_str());
//    CCLOG("cur animation size %lu", _currentAnimation.size());
//    CCLOG("==================");
    
    if(_currentAnimation.size() <= 0) return;
    
    float frameTime = (1.0f / _frameRate);
    _timeElapsed += dt;
    
    if(_timeElapsed >= frameTime)
    {
        _animSprite->setSpriteFrame(_currentAnimation.at(_frameIndex).c_str());
    
        _timeElapsed = 0;
        _frameIndex++;
        
    
        if( _frameIndex == _currentAnimation.size() && _onAnimFinished != nullptr ) _onAnimFinished();
        
        if(_loop && _frameIndex == _currentAnimation.size() ) _frameIndex = 0;
        
        _frameIndex = (int)clampf(_frameIndex, 0, (_currentAnimation.size() - 1) );
    
    }
    
}

void AnimationComponent::addAnimation( std::string animationName, std::vector<std::string> frames)
{
    _animationsData.insert( std::pair< std::string, std::vector<std::string> >( animationName, frames ) );
}

void AnimationComponent::playAnimation(std::string animationName, int frameRate, bool loop, bool randStart)
{
    for(auto itr = _animationsData.begin(); itr != _animationsData.end(); itr++)
    {
        if(animationName.compare( itr->first ) == 0 )
        {
            _currentAnimation = itr->second;
        }
    }
    
    _curAnimationName = animationName;
    _loop = loop;
    _frameRate = frameRate;
    _frameIndex = (!randStart) ? 0 : _currentAnimation.size() * CCRANDOM_0_1();
}

void AnimationComponent::stopAnimation()
{
    _timeElapsed = 0;
    _frameIndex = 0;
    _frameRate = 0;
    _loop = false;
    _curAnimationName = "";
    _currentAnimation.clear();
}

void AnimationComponent::onAnimationFinished(std::function<void ()> onFinnished)
{
    _onAnimFinished = onFinnished;
}

std::string AnimationComponent::getCurrentAnimationName()
{
    return _curAnimationName;
}


