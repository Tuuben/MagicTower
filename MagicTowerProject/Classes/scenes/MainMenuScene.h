//
//  MainMenuScene.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 11/05/2017.
//
//

#ifndef MainMenuScene_hpp
#define MainMenuScene_hpp

#include <stdio.h>
USING_NS_CC;
class MainMenuScene : public Layer {
    
public:
    static Scene* createScene();
    CREATE_FUNC(MainMenuScene);
    bool init();
    
private:
    Size visibleSize;
    Label* titleLabel;
    
    Sprite* playButton;
    Sprite* storeButton;
    void createWallTiles();
};

#endif /* MainMenuScene_hpp */
