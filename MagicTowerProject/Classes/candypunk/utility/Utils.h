//
//  Utils.hpp
//  Ghostly
//
//  Created by Tobias Helsing on 24-01-2017.
//
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include "cocos2d.h"

class Utils{

public:
    static cocos2d::Vec2 lerp( cocos2d::Vec2 start, cocos2d::Vec2 end, float percentage ){
        return start + ( (end - start) * percentage );
    };
    
    static float distance( cocos2d::Vec2 v1, cocos2d::Vec2 v2 ){
        Vec2 diff = v2 - v1;
        return sqrtf( powf(diff.x, 2) + powf(diff.y, 2) );
    }
    
    static cocos2d::Vec2 roundVec( cocos2d::Vec2 v ){
        return Vec2( round(v.x), round(v.y) );
    }
    
    static bool rayCastDirection( Vec2 startWorldPos, Vec2 direction, int tag, bool debugDraw = false ){
    
        Vec2 endPos = Vec2( startWorldPos.x + direction.x, startWorldPos.y + direction.y );
        bool returnValue = false;
        PhysicsRayCastCallbackFunc func = [&tag, &returnValue](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool
        {
            
            PhysicsBody* body = info.shape->getBody();
            
            if(body->getCollisionBitmask() == tag) {
                CCLOG("EY COLLIDE");
                returnValue = true;
                return true;
            }
            
            return false;
        };
        
        if(debugDraw){
            
            auto drawNode = DrawNode::create();
            drawNode->drawLine(startWorldPos, endPos, Color4F::RED);
            drawNode->setName("debug_ray");
            Director::getInstance()->getRunningScene()->addChild(drawNode);
            
        }
  
        Director::getInstance()->getRunningScene()->getPhysicsWorld()->rayCast(func, startWorldPos, endPos, nullptr);
        
        return returnValue;
    }
    
    
};

#endif /* Utils_hpp */
