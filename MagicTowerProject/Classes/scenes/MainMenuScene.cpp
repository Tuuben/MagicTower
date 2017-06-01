//
//  MainMenuScene.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 11/05/2017.
//
//

#include "MainMenuScene.h"
#include "../Globals.h"
Scene* MainMenuScene::createScene(){
    
    Scene* scene = Scene::create();
    MainMenuScene* layer = MainMenuScene::create();
    
    scene->addChild(layer);
    
    return scene;

}

bool MainMenuScene::init(){

    if(!Layer::init()){
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    
    titleLabel = Label::createWithTTF("TOWER OF THE DEAD", "fonts/alagard.ttf", 18);
    titleLabel->getFontAtlas()->setAliasTexParameters();
    titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.72f));
    this->addChild(titleLabel);
    
    auto moveTo = EaseSineInOut::create(MoveTo::create(1.8, Vec2(visibleSize.width / 2, visibleSize.height * 0.71f)));
    auto moveBack = EaseSineInOut::create(MoveTo::create(1.8, Vec2(visibleSize.width / 2, visibleSize.height * 0.72f)));
    auto seq = Sequence::create(moveTo, moveBack, NULL);
    auto rep = RepeatForever::create(seq);
    titleLabel->runAction(rep);
    
    playButton = Sprite::createWithSpriteFrameName(BUTTON_LARGE);
    playButton->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.5f));
    playButton->setScale(2.0f);
    auto playTxt = Label::createWithTTF("PLAY", "fonts/alagard.ttf", 8);
    playTxt->getFontAtlas()->setAliasTexParameters();
    playTxt->setColor(Color3B::BLACK);
    playTxt->setPosition(Vec2(playButton->getContentSize().width / 2, playButton->getContentSize().height * 0.54f));
    playButton->addChild(playTxt);
    this->addChild(playButton);
    
    storeButton = Sprite::createWithSpriteFrameName(BUTTON_LARGE);
    storeButton->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 0.41f));
    storeButton->setScale(2.0f);
    auto storeTxt = Label::createWithTTF("STORE", "fonts/alagard.ttf", 8);
    storeTxt->getFontAtlas()->setAliasTexParameters();
    storeTxt->setColor(Color3B::BLACK);
    storeTxt->setPosition(Vec2(storeButton->getContentSize().width / 2, storeButton->getContentSize().height * 0.54f));
    storeButton->addChild(storeTxt);
    this->addChild(storeButton);
    
    createWallTiles();
    
    
    
    CCLOG("wha");
    return true;
    
}

void MainMenuScene::createWallTiles(){

    for(int i = 0; i < 30; i++){
    
        // create walls
        
    }

}



