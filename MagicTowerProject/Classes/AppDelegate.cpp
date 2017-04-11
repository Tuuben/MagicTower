#include "AppDelegate.h"
#include "scenes/GameScene.h"
#include "Globals.h"
#include <SimpleAudioEngine.h>
#include "handlers/AudioManager.h"

USING_NS_CC;
//240 320
//192 256
//static cocos2d::Size designResolutionSize = cocos2d::Size(192, 256);
static cocos2d::Size designResolutionSize = cocos2d::Size(320, 480);
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("MagicTowerProject", Rect(0, 0,
        designResolutionSize.width * 2 , designResolutionSize.height * 2));
        
#else
        glview = GLViewImpl::create("MagicTowerProject");
#endif
        director->setOpenGLView(glview);
    }

    //cache sprites
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(TILEMAP_PLIST_PATH);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(ACTOR_PLIST_PATH);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(SPRITEMAP_PLIST_PATH);
    
    //alias hack's
    auto aaSpriteMapSprite = Sprite::createWithSpriteFrameName(PARTICLE_CIRCLE_01);
    aaSpriteMapSprite->getTexture()->setAliasTexParameters();
    auto aaTileMapSprite = Sprite::createWithSpriteFrameName(SOLID_TILE_SPRITE_00);
    aaTileMapSprite->getTexture()->setAliasTexParameters();
    auto aaActorsSprite = Sprite::createWithSpriteFrameName("blocky_idle_01.png");
    aaActorsSprite->getTexture()->setAliasTexParameters();
    
    //cache audio
    AudioManager::getInstance()->preloadAudio({
        SFX_HIT_01,
        SFX_HIT_02,
        SFX_PICK_UP_01,
        SFX_PICK_UP_02,
        SFX_PICK_UP_03,
        SFX_PICK_UP_04,
        SFX_PICK_UP_05,
        SFX_JUMP_01
    });
    
    // turn on display FPS
    ///director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);

    // create a scene. it's an autorelease object
    auto scene = GameScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
     CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
