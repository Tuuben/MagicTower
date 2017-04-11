//
//  Actor.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 11/04/2017.
//
//

#include "Actor.h"

bool Actor::init(){
    
    if(!Node::init()){ return false; }
    
    
    return true;

}

void Actor::update(float dt){

    if(frozen){
        return;
    }
    
    Node::update(dt);
    
}


void Actor::freeze(){
    
    frozen = true;
    
    if( getPhysicsBody() != nullptr ){
        getPhysicsBody()->setEnabled(false);
    }
    
    this->stopAllActions();
    
}

void Actor::unfreeze(){
    
    frozen = false;
    
    if( getPhysicsBody() != nullptr ){
        getPhysicsBody()->setEnabled(true);
    }
    
}
