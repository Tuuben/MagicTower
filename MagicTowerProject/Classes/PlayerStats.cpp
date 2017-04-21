//
//  PlayerStats.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 21/04/2017.
//
//

#include "PlayerStats.h"

PlayerStats* PlayerStats::instance = nullptr;
PlayerStats* PlayerStats::getInstance(){
    
    if(instance == nullptr)
        instance = new PlayerStats();
    
    return instance;

}
