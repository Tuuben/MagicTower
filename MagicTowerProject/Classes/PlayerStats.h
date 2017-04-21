//
//  PlayerStats.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 21/04/2017.
//
//

#ifndef PlayerStats_hpp
#define PlayerStats_hpp

#include <stdio.h>

class PlayerStats {
    
public:
    static PlayerStats* getInstance();
    int getLives(){ return lives; }
    
private:
    int lives = 3;
    static PlayerStats* instance;
    
};

#endif /* PlayerStats_hpp */
