//
//  Player.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 13-02-2016.
//
//

#ifndef Player_h
#define Player_h

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../Globals.h"
#include "SimpleParticle.h"
#include "../candypunk/components/AnimationComponent.h"

USING_NS_CC;

class Player : public cocos2d::Node {
#define PLAYER_COLOR cocos2d::Color3B(255, 255, 255)
#define PLAYER_CONTENT_SIZE cocos2d::Size(10, 10)
#define JUMP_FORCE 80
#define HORIZONTAL_SPEED 40
#define IDLE_ANIMATION_NAME "idle_anim"
#define FLY_ANIMATION_NAME "fly_anim"
#define FALL_ANIMATION_NAME "fall_anim"
#define JUMP_ANIMATION_NAME "jmp_anim"
#define EAT_ANIMATION_NAME "eat_anim"
    
public:
    static Player* create( cocos2d::Layer* onLayer );
    bool init(cocos2d::Layer* onLayer);
    
private:
    void setupEvents();
    void update(float dt);
    void jump();
    void checkSideCollisions();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void onContactSeparate( cocos2d::PhysicsContact& contact );
    void setupAnimations();
    void resetAnimation();
    void collisionEmit( cocos2d::Vec2& pos, int xDir = 1, int yDir = 1 );
    void emitParticles(std::string sprite, cocos2d::Color3B color, cocos2d::Vec2& pos, int num, float vx, float vy, float lifeTime,float endScale = 1);
    
    Player(){}
    ~Player(){}
  
private:
    cocos2d::Sprite* _sprite;
    cocos2d::Size _visibleSize;
    cocos2d::Layer* _onLayer;
    AnimationComponent* _animComponent;
    
    int _direction = 1;
    int _collisionCount = 0;
    bool _touchingLeft = false;
    bool _touchingRight = false;
    bool _touchBottom = false;
    bool _touchTop = false;
    bool _isWaiting = true;
    
    cocos2d::DrawNode* _drawNode;
    float _angle = 0;
    
    std::vector<std::string> _idleAnimationFrames;
    std::vector<std::string> _dustParticleFrames;
};

#endif /* Player_hpp */
