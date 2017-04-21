//
//  HealthBar.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 21/04/2017.
//
//

#ifndef HealthBar_hpp
#define HealthBar_hpp

#include <stdio.h>
#include "../../cocos2d/cocos/cocos2d.h"
USING_NS_CC;

class HealthBar : public Node{
    
public:
    CREATE_FUNC(HealthBar);
    bool init();
    void removeHeart();
    
private:
    std::vector<Sprite*> heartList;
    
    void createHealthBar();
    void alignBar();
};

#endif /* HealthBar_hpp */
