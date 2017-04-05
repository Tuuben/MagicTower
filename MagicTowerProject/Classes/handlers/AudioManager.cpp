//
//  AudioManager.cpp
//  CocosTemplateProject
//
//  Created by Tobias Helsing on 25/11/15.
//
//

#include "AudioManager.h"

AudioManager* AudioManager::_instance = 0;

AudioManager::AudioManager(){}
AudioManager::~AudioManager()
{
    
}

AudioManager* AudioManager::getInstance()
{
    if(_instance == 0)
        _instance = new AudioManager();
    
    return _instance;
}

void AudioManager::preloadAudio(std::vector<const char*> paths)
{
    for(int i = 0; i < paths.size(); i++)
    {
        preloadAudio(paths.at(i));
    }
}

void AudioManager::preloadAudio( const char* path)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(path);
    m_audioPreloaded = true;
}

int AudioManager::playSoundEffect( const char* filePath, float volume, float pitch, bool loop )
{
    if(!m_audioPreloaded)
    {
        return 0;
        CCLOGERROR("No Audio preloaded ...");
    }
    
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    
    int id = audio->playEffect(filePath, loop, pitch);
    
    if ( getGlobalVolume() <= 0 )
    {
        volume = 0;
    }
    
    audio->setEffectsVolume(volume);
    
    return id;
}

void AudioManager::pauseSoundEffect( uint id )
{
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->pauseEffect(0);
}

void AudioManager::resumeSoundEffect( uint id )
{
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->resumeEffect(id);
}

void AudioManager::stopAllSoundEffects()
{
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->stopAllEffects();
}

float AudioManager::_volume = 1.0f;
void AudioManager::setGlobalVolume(float vol)
{
    _volume = vol;
}

float AudioManager::getGlobalVolume()
{
    return _volume;
}

