//
//  World.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 19/02/16.
//
//

#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include "../cocos2d/cocos/cocos2d.h"

/** @class World
 * @brief
 * All scenes that need:
 * - Collisions
 * - Basic physics
 * - Camera
 * Can extend World 
 *
 * The world will add (2) cocos2d::Layers
 * 1. ObjectLayer
 * 2. UILayer
 *
 * Set camera position will modify the position of ObjectLayer
 */
class World : public cocos2d::Layer
{
    
public:
    
    bool init();
    
    /**
     * @brief Get the object layer.
     * @return Get Node layer.
     */
    cocos2d::Layer* getObjectLayer();
    
    /**
     * @brief Get the object layer.
     * @return Get UI layer.
     */
    cocos2d::Layer* getUILayer();
    
    /**
     * @brief update world
     */
    void update(float dt);
    
    /**
     * @brief add node to world
     */
    void addObject( cocos2d::Node* node, int zLayer = 0);
    
    /**
     * @brief add node to world
     */
    void addUIElement( cocos2d::Node* node, int zLayer = 0);
    
    /**
     * @brief Set the camera position with vec2.
     * @param Sets the X,Y position of camera.
     */
    void setCameraPosition( cocos2d::Vec2 position );
    
    /**
     * @brief Set the camera x position with float.
     * @param Sets the X position of camera.
     */
    void setCameraPositionX( float x );
    
    /**
     * @brief Set the camera y position with float.
     * @param Sets the Y position of camera.
     */
    void setCameraPositionY( float y );
    
    /**
     * @brief Get the camera position.
     * @return Vec2 Position.
     */
    cocos2d::Vec2 getCameraPosition();
    
    /**
     * @brief Get the camera position.
     * @return float X position
     */
    float getCameraPositionX();
    
    /**
     * @brief Get the camera position.
     * @return float Y position
     */
    float getCameraPositionY();
    
    /**
     * @brief set camera shake
     */
    void setScreenShake( float time, float xForce, float yForce );
    
    /**
     * @brief updates the screenshake if it has been set
     */
    void updateScreenShake();
    
protected:
    World(){}
    ~World(){}
    
private:
    void debugDraw( cocos2d::Vec2 pos, cocos2d::Vec2 size);
    
private:
    static World* _instance;
    cocos2d::Layer* _objectLayer = nullptr;
    cocos2d::Layer* _uiLayer = nullptr;
    
    float _shakeTime = 0;
    float _xShakeForce = 0;
    float _yShakeForce = 0;

};

#endif /* World_hpp */
