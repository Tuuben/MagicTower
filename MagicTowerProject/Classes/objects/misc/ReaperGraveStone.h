//
//  GraveStone.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 19/04/2017.
//
//

#ifndef ReaperGraveStone_hpp
#define ReaperGraveStone_hpp

#include <stdio.h>
#include "../../../cocos2d/cocos/cocos2d.h"
USING_NS_CC;

class ReaperGraveStone : public Node {
    
public:
    CREATE_FUNC(ReaperGraveStone);
    bool init();
private:
    Sprite* sprt;
};

#endif /* GraveStone_hpp */
