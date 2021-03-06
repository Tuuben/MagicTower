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
#include "../Globals.h"
#include "SimpleParticle.h"
#include "../candypunk/components/AnimationComponent.h"
#include "../candypunk/objects/Actor.h"


USING_NS_CC;

class Player : public Actor {
#define PLAYER_COLOR cocos2d::Color3B(255, 255, 255)
#define PLAYER_CONTENT_SIZE cocos2d::Size(10, 10)
#define JUMP_FORCE 80
#define HORIZONTAL_SPEED 40
#define IMMORTAL_DURATION 2.0f
#define IDLE_ANIMATION_NAME "idle_anim"
#define FLY_ANIMATION_NAME "fly_anim"
#define FALL_ANIMATION_NAME "fall_anim"
#define JUMP_ANIMATION_NAME "jmp_anim"
#define EAT_ANIMATION_NAME "eat_anim"
    
public:
    static Player* create( cocos2d::Layer* onLayer );
    bool init(cocos2d::Layer* onLayer);
    void removeHealth();
    void addSoul(Node* soul);
    void freeze() override;
    void unfreeze() override;
    
private:
    void setupEvents();
    void setStats();
    void update(float dt) override;
    void jump();
    void immortalCountdown(float dt);
    void kill();
    void checkSideCollisions();
    bool onTouchesBegan(std::vector<Touch*> touches, cocos2d::Event* event);
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
    
    int _lives = 3;
    int _direction = 1;
    float _immortalDuration = 0.0f;
    int _collisionCount = 0;
    bool _immortal = true;
    bool _touchingLeft = false;
    bool _touchingRight = false;
    bool _touchBottom = false;
    bool _touchTop = false;
    bool _isWaiting = true;
    bool _isDead = false;
    
    cocos2d::DrawNode* _drawNode;
    float _angle = 0;
    std::vector<Node*> _soulList;
    std::vector<std::string> _idleAnimationFrames;
    std::vector<std::string> _dustParticleFrames;
    
};

#endif /* Player_hpp */
