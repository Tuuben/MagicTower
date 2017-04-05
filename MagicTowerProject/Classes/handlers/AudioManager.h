//
//  AudioManager.hpp
//  CocosTemplateProject
//
//  Created by Tobias Helsing on 25/11/15.
//
//

#ifndef AudioManager_hpp
#define AudioManager_hpp

#include <SimpleAudioEngine.h>
#include "../../cocos2d/cocos/cocos2d.h"
#include "../Globals.h"

class AudioManager
{
public:
    static AudioManager* getInstance();
    
    void preloadAudio( std::vector<const char*> paths );
    void preloadAudio( const char* path);
    
    int playSoundEffect( const char* filePath, float volume, float pitch = 1.0f, bool loop = false);
    void pauseSoundEffect( uint id );
    void resumeSoundEffect( uint id );
    void stopAllSoundEffects();
    bool isPlaying( const char* filePath );
    void setGlobalVolume(float vol);
    float getGlobalVolume();
    
private:
    static AudioManager* _instance;
    static float _volume;

    bool m_audioPreloaded = false;
    
    AudioManager();
    ~AudioManager();
};

#endif /* AudioManager_hpp */
