//
//  Food.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 23/02/16.
//
//

#ifndef Food_h
#define Food_h

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../Globals.h"
#include "Pickup.h"

class Food : public Pickup {
    
public:
    static Food* create();
    bool init() override;
    
    void onPickUpObject() override;
    void destroySelf() override;
    Food(){};
    ~Food(){};
    
private:
    cocos2d::Sprite* _sprite;

};

#endif /* Food_hpp */
