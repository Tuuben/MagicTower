//
//  World.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 19/02/16.
//
//

#include "World.h"

using namespace cocos2d;
World* World::_instance = nullptr;
bool World::init()
{
    //add layers
    _objectLayer = Layer::create();
    _objectLayer->setGlobalZOrder(0);
    
    _uiLayer = Layer::create();
    _uiLayer->setGlobalZOrder(100);
    
    this->addChild(_objectLayer);
    this->addChild(_uiLayer);
    
    _instance = this;
    
    this->scheduleUpdate();
    
    return true;
}

cocos2d::Layer* World::getObjectLayer(){ return _objectLayer; }
cocos2d::Layer* World::getUILayer(){ return _uiLayer; }

void World::update(float dt)
{
    
    _shakeTime -= dt;
    
    if(_shakeTime > 0)
    {
        updateScreenShake();
    }
}

void World::addObject(cocos2d::Node *node, int zLayer)
{
    _objectLayer->addChild(node, zLayer);
}

void World::addUIElement(cocos2d::Node *node, int zLayer)
{
    _uiLayer->addChild(node, zLayer);
}

void World::setCameraPosition(cocos2d::Vec2 position)
{
    _objectLayer->setPosition( Vec2( -position.x , -position.y ) );
}

void World::setCameraPositionX(float x)
{
    _objectLayer->setPositionX( -x );
}

void World::setCameraPositionY(float y)
{
    
    _objectLayer->setPositionY(-y);
}

Vec2 World::getCameraPosition()
{
    return Vec2( fabsf( _objectLayer->getPositionX() ), fabsf( _objectLayer->getPositionY() ) );
}

float World::getCameraPositionX()
{
    return fabsf( _objectLayer->getPositionX() );
}

float World::getCameraPositionY()
{
    return fabsf( _objectLayer->getPositionY() );
}

void World::setScreenShake(float time, float xForce, float yForce)
{
    _shakeTime = time;
    _xShakeForce = xForce;
    _yShakeForce = yForce;
}

void World::updateScreenShake()
{
    float randXShake = getCameraPositionX() + ( _xShakeForce * CCRANDOM_0_1() - (_xShakeForce / 2) );
    float randYShake = getCameraPositionY() + ( _yShakeForce * CCRANDOM_0_1() - (_yShakeForce / 2) );
    setCameraPosition( Vec2( randXShake, randYShake ) );
}



