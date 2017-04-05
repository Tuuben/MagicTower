//
//  Coin.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 26/02/16.
//
//

#ifndef Coin_hpp
#define Coin_hpp

#include "../Globals.h"
#include "Pickup.h"

class Coin : public Pickup {
    
public:
    static Coin* create();
    bool init() override;
    void onPickUpObject() override;
    void destroySelf() override;
    
protected:
    Coin(){}
    ~Coin(){}
};

#endif /* Coin_hpp */
