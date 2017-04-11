//
//  ClutterTile.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 17/02/16.
//
//

#include "ClutterTile.h"

using namespace cocos2d;

ClutterTile* ClutterTile::create(ClutterType clutterType)
{
    ClutterTile* _cTile = new ClutterTile();
    
    if(_cTile && _cTile->init(clutterType))
    {
        _cTile->autorelease();
        return _cTile;
    }

    CC_SAFE_DELETE(_cTile);
    return NULL;
}

bool ClutterTile::init(ClutterType clutterType)
{
    std::string clutterSpritePath = GRASS_SPRITE_01;
 
    _clutterType = clutterType;
    
    _animComp = AnimationComponent::createComponent(this);
    this->addComponent(_animComp);
    
    if(clutterType == ClutterType::GRASS) {
        
        initWithSpriteFrameName(clutterSpritePath);
        
        _animComp->addAnimation( "idle", { "grass_clutter_01.png", "grass_clutter_02.png", "grass_clutter_03.png", "grass_clutter_04.png" } );
        _animComp->playAnimation( "idle", 8, true, true );
        
        this->setGlobalZOrder(-50);
        
        Color3B primaryColor(0, 255, 0);
        this->setColor( primaryColor );
        
    }
    else if(clutterType == ClutterType::BANNER){
        
        initWithSpriteFrameName( BANNER_01 );
        
        _animComp->addAnimation( "idle", { BANNER_01, BANNER_02, BANNER_03, BANNER_04, BANNER_05, BANNER_06, BANNER_07, BANNER_08 } );
        _animComp->playAnimation( "idle", 12 );
        
        this->setGlobalZOrder(40);
        
        this->setScale(-1, 1);
        this->setColor( Color3B(255, 230, 183) );

    }
    
    
    return true;
}
