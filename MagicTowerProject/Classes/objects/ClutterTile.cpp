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
        Color3B secondaryColor(216, 248, 120);
        this->setColor( secondaryColor );//( (100 * CCRANDOM_0_1()) < 50 ) ? primaryColor : secondaryColor );
        
    }
    else if(clutterType == ClutterType::BACKGROUND){
        
        std::stringstream clutterSpritePath;
        clutterSpritePath << "wall_clutter_" << (int)( 11 * CCRANDOM_0_1() + 1 ) << ".png";
        
        initWithSpriteFrameName(clutterSpritePath.str());
        this->setColor( Color3B(77, 77, 77));
        this->setGlobalZOrder(-100);
        this->setPositionZ(-10);
    }
    
    
    return true;
}
