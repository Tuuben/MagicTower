//
//  AnimationComponent.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 18/02/16.
//
//

#ifndef AnimationComponent_h
#define AnimationComponent_h

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"

class AnimationComponent : public cocos2d::Component
{

public:
    static AnimationComponent* createComponent(cocos2d::Sprite* animateSprite);
    bool init(cocos2d::Sprite* animateSprite);
    void update(float dt) override;
    void addAnimation( std::string animationName, std::vector<std::string> frames );
    void playAnimation( std::string animationName, int frameRate, bool loop = true );
    void stopAnimation();
    void onAnimationFinished( std::function<void()> onFinnished );
    std::string getCurrentAnimationName();
    
private:
    AnimationComponent(){}
    ~AnimationComponent(){}
    
private:
    cocos2d::Sprite* _animSprite;
    std::map< std::string, std::vector<std::string> > _animationsData;
    std::vector< std::string > _currentAnimation;
    std::string _selectedAnimationName;
    float _timeElapsed = 0;
    int _frameIndex = 0;
    int _frameRate = 0;
    bool _loop = false;
    std::string _curAnimationName = "";
    std::function<void()> _onAnimFinished = nullptr;
};

#endif /* AnimationComponent_h */
