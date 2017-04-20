//
//  MapHandler.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 13-02-2016.
//
//

#include "MapHandler.h"
#include "../objects/Player.h"
#include "../objects/Food.h"
#include "../objects/Coin.h"
#include "../objects/traps/Swing.h"
#include "../objects/enemys/Bat.h"
#include "../objects/traps/ExplosiveCrate.h"
#include "../objects/ClutterObject.h"
#include "../objects/GraveStone.h"


using namespace cocos2d;

MapHandler* MapHandler::create()
{
    MapHandler* _MapHandler = new MapHandler();
    
    if(_MapHandler && _MapHandler->init()){
    
        _MapHandler->autorelease();
        return _MapHandler;
        
    }
    
    CC_SAFE_DELETE(_MapHandler);
    return NULL;
}

bool MapHandler::init() {

    _visibleSize = Director::getInstance()->getVisibleSize();
    
    createLevelBaseFloor();
    createLevelBlockOutline();
    
    //alias hack
    auto aspr = Sprite::createWithSpriteFrameName(SOLID_TILE_SPRITE_01);
    aspr->getTexture()->setAliasTexParameters();
    
    return true;
}

void MapHandler::updateMap( float clearPositionY )
{
    // Reset variables
    _swingsSpawned = 0;
    _batsSpawned = 0;
    
    // Update block height
    _baseYIndex += LEVEL_BLOCK_HEIGHT;
    
    // Removes old tiles below the specified position
    clearOld(clearPositionY);
    
    // Creates the block outline
    createLevelBlockOutline();
    
    // Randomizes and creates a path for the player
    createLevelContentData();
    
    // Clears the path for the player
    clearPathInContentData();
    
    // Creates level objects
    createLevelContentObjects();
    
    // Creates large level objects
    createLargeLevelContentObjects();
    
}

float MapHandler::getMapWidth()
{
    return LEVEL_BLOCK_WIDTH * TILE_WIDTH;
}

float MapHandler::getMapHeight()
{
    return _baseYIndex * TILE_WIDTH;
}

void MapHandler::createLevelBaseFloor()
{
    
    // Create tiles
    for(int i = 1; i < LEVEL_BLOCK_WIDTH; i++)
    {
        createSolidTile(i, BASE_FLOOR_HEIGHT, 0);
    }
    
    // Create statue
    reaperStone = ReaperGraveStone::create();
    reaperStone->setPosition(Vec2( (LEVEL_BLOCK_WIDTH / 2) * TILE_WIDTH, 42.0f ));
    addChild(reaperStone);
    
}

void MapHandler::createLevelBlockOutline()
{
    
    for(int i = _baseYIndex; i < _baseYIndex + LEVEL_BLOCK_HEIGHT; i++)
    {
        
        createSolidTile(-1, i, 15);
        createSolidTile(-2, i, 15);
        createOuterTile( 0, i, -1);
        createOuterTile( LEVEL_BLOCK_WIDTH, i, 1);
        createSolidTile( LEVEL_BLOCK_WIDTH + 1, i, 15);
        createSolidTile( LEVEL_BLOCK_WIDTH + 2, i, 15);
        
    }
    
}

void MapHandler::createLevelContentData()
{
    //handle map data
    if( mapData.size() > 0 ) {
        mapData.clear();
    }
    
    //rezise mapdata
    mapData.resize(LEVEL_BLOCK_WIDTH);
    for(int mapItr = 0; mapItr < LEVEL_BLOCK_WIDTH; mapItr++)
    {
        mapData[mapItr].resize(LEVEL_BLOCK_HEIGHT);
    }

    //data iteration
    int numObjectsSpawned = 0;
    for(int y = LEVEL_DEAD_ZONE_BOTTOM; y < LEVEL_BLOCK_HEIGHT - LEVEL_DEAD_ZONE_TOP; y++)
    {

        int sourceX = (int)(CCRANDOM_0_1() * LEVEL_BLOCK_WIDTH - 2) + 2;
        float rand = 100 * CCRANDOM_0_1();
            
        if(rand < SOLID_SPAWN_CHANCE && numObjectsSpawned < MAX_TILE_SPAWNS)
        {
            mapData[sourceX][y] = SOLID_TILE_ID;
            numObjectsSpawned++;
            
            int clusterWidth = (6 * CCRANDOM_0_1() + 2);
            int clusterHeight = (3 * CCRANDOM_0_1() + 1);
                
            for(int cX = sourceX - (clusterWidth / 2); cX < sourceX + (clusterWidth / 2); cX++)
            {
                    
                for( int cY = y; cY < y + clusterHeight; cY++ ){
                        
                    int spawnX = clampf(cX, 0, LEVEL_BLOCK_WIDTH - 2);
                    int spawnY = clampf(cY, 0, LEVEL_BLOCK_HEIGHT - 1);
                
                    mapData[spawnX][spawnY] = 1;
                    numObjectsSpawned++;
                        
                }
                    
            }
        }
        
    }
    
}

void MapHandler::clearPathInContentData(){

    int gaps[2];
    gaps[0] = ((LEVEL_BLOCK_WIDTH - 4) * CCRANDOM_0_1()) + 4;
    gaps[1] = gaps[0] + 1;
    
    for(int y = 0; y < LEVEL_BLOCK_HEIGHT; y++){
        for(int x = 0; x < LEVEL_BLOCK_WIDTH; x++){
        
            if(x == gaps[0] || x == gaps[1] ){
                mapData[x][y] = 0;
            }
            
        }
    }

}

void MapHandler::createLevelContentObjects()
{
    
    for(int y = 0; y < LEVEL_BLOCK_HEIGHT; y++)
    {
        for(int x = 1; x < LEVEL_BLOCK_WIDTH; x++)
        {
            int top = ( y == LEVEL_BLOCK_HEIGHT - 1 ) ? 0 : y + 1;
            int bottom = ( y == 0 ) ? 0 : y - 1;
            int right = (x == LEVEL_BLOCK_WIDTH - 1) ? 0 : x + 1;
            int left = (x == 0) ? 0 : x - 1;
            
            int tileIndex = getTileIndex(mapData[x][top], mapData[x][bottom], mapData[left][y], mapData[right][y]);
            
            if(mapData[x][y] == SOLID_TILE_ID)
            {
                //create solid tile
                if(tileIndex <= 15)
                {
                    createSolidTile(x, y + _baseYIndex, tileIndex);
                }
                
                // create swing traps
                if( (tileIndex < 15) &&
                    ((100 * CCRANDOM_0_1()) < SWING_TRAP_SPAWN_CHANCE) &&
                    (x > 4) && (x < LEVEL_BLOCK_WIDTH - 4) &&
                    (y > LEVEL_DEAD_ZONE_BOTTOM) ){
                
                    createSwing(x, y + _baseYIndex);
                    
                }
            }
            
            if(mapData[x][y] == EMPTY_TILE_ID)
            {
                int spotFilled = 0;
                
                //create spikes
                if(tileIndex < 15 && tileIndex > 0)
                {
                    if((100 * CCRANDOM_0_1()) < SPIKE_SPAWN_CHANCE)
                    {
                        switch (tileIndex) {
                                
                            case 1:{
                                createSpike(x, y + _baseYIndex, Spike::DIRECTION::BOTTOM);
                                
                                spotFilled++;
                                break;
                            }
                            case 2:{
                                createSpike(x, y + _baseYIndex, Spike::DIRECTION::RIGHT);
                                
                                spotFilled++;
                                break;
                            }
                            case 4:
                                createSpike(x, y + _baseYIndex, Spike::DIRECTION::TOP);
                                
                                spotFilled++;
                                break;
                                
                            case 8:
                                createSpike(x, y + _baseYIndex, Spike::DIRECTION::LEFT);
                                
                                spotFilled++;
                                break;
                        }
                    }
                    else if((100 * CCRANDOM_0_1()) < EXPLOSIVE_SPAWN_CHANCE){
                        
                        createExplosive(x, y + _baseYIndex);
                        spotFilled++;
                        
                    }
                }
                
                if( (tileIndex == 4 || tileIndex == 5 || tileIndex == 6 || tileIndex == 12) && spotFilled < 1)
                {
                    // Create clutter grass
                    if( (100 * CCRANDOM_0_1()) < GRASS_SPAWN_CHANCE)
                    {
                        
                        createClutterTile(x, y + _baseYIndex, ClutterType::GRASS );
                        
                    }
                    // Create clutter objects
                    else if( (100 * CCRANDOM_0_1() < CLUTTER_OBJECT_SPAWN_CHANCE)){
                    
                        createClutterObject(x, y + _baseYIndex);
                        
                    }
                    
                    // Create grave stones
                    if( (100 * CCRANDOM_0_1()) < GRAVE_SPAWN_CHANCE){
//                        createGraveStone(x, y + _baseYIndex);
                    }
                }
                
                // Create banenrs
                if( tileIndex == 1 && spotFilled < 1){
                    
                    if( (100 * CCRANDOM_0_1()) < BANNER_SPAWN_CHANCE ){
                        
                        createClutterTile(x, y + _baseYIndex, ClutterType::BANNER);
                        
                    }
                    
                }
                
                if(tileIndex == 0 && spotFilled < 1)
                {
                    
                    if((100 * CCRANDOM_0_1()) < COIN_SPAWN_CHANCE){
                        createCoin(x, y + _baseYIndex);
                    }
                    else if( (100 * CCRANDOM_0_1()) < FOOD_SPAWN_CHANCE){
                        createFood(x, y + _baseYIndex);
                    }
                    
                    if( (100 * CCRANDOM_0_1()) < BAT_SPAWN_CHANCE && y > LEVEL_DEAD_ZONE_BOTTOM) {
                        createBat(x, y + _baseYIndex);
                    }
                }
                
            }
            
        }
    }
    
}

void MapHandler::createLargeLevelContentObjects(){
    
    for(int y = 0; y < LEVEL_BLOCK_HEIGHT - 10; y++)
    {
        for(int x = 0; x < LEVEL_BLOCK_WIDTH - 14; x++)
        {
    
            //Check grid
            int bX = x;
            int bY = y;
            bool foundSolidInGrid = false;
            for(int checkY = bY; checkY < bY + 10; checkY++){
                for(int checkX = bX; checkX < bX + 14; checkX++){
                    
                    if( mapData[checkX][checkY] == SOLID_TILE_ID )
                        foundSolidInGrid = true;
                    
                }
            }
            
            if(!foundSolidInGrid){
                int halfGridHeight = 6;
                
                if( 100 * CCRANDOM_0_1() < SWING_TRAP_SPAWN_CHANCE){
                    int xIndex = bX + (4 * CCRANDOM_0_1()) + 6;
                    int yIndex = bY + halfGridHeight;
                    createSwing( xIndex, yIndex + _baseYIndex );
                    mapData[xIndex][yIndex] = 1;
                    
                }
                
            }
          
            
        }
    
    }
    
}

int MapHandler::getTileIndex(int top, int bottom, int left, int right)
{
    int sum = 0;
    
    if(top == 1) sum += 1;
    if(left == 1) sum += 2;
    if(bottom == 1) sum += 4;
    if(right == 1) sum += 8;
    
    return sum;
}

void MapHandler::clearOld( float clearPositionY )
{
    
    if(_mapNodes.size() > 0)
    {
        for(int i = 0; i < _mapNodes.size(); i++)
        {
            if(_mapNodes.at(i) == NULL){
                _mapNodes.erase( _mapNodes.begin() + i );
            }
            
        }
    }
    
    
    for(int i = _mapNodes.size() - 1; i >= 0; i-- )
    {
        
        if(dynamic_cast< Node* >(_mapNodes.at(i)) != nullptr)
        {
            if(_mapNodes.at(i)->getPositionY() < clearPositionY  )
            {
                this->removeChild( _mapNodes.at(i) );
                _mapNodes.erase( _mapNodes.begin() + i );
            }
        }
        
    }
    
    for(int i = _movingObjects.size() - 1; i >= 0; i-- )
    {
        
        if(dynamic_cast< Actor* >(_movingObjects.at(i)) != nullptr)
        {
            if(_movingObjects.at(i)->getPositionY() < clearPositionY  )
            {
                this->removeChild( _movingObjects.at(i) );
                _movingObjects.erase( _movingObjects.begin() + i );
            }
        }
        
    }
    
}

void MapHandler::createSolidTile(int xIndex, int yIndex, int tileIndex)
{
    //create tile
    std::string spritePath = SOLID_TILE_SPRITE_00;
    
    switch (tileIndex) {
        case 1:
            spritePath = SOLID_TILE_SPRITE_01;
            break;
        case 2:
            spritePath = SOLID_TILE_SPRITE_02;
            break;
        case 3:
            spritePath = SOLID_TILE_SPRITE_03;
            break;
        case 4:
            spritePath = SOLID_TILE_SPRITE_04;
            break;
        case 5:
            spritePath = SOLID_TILE_SPRITE_05;
            break;
        case 6:
            spritePath = SOLID_TILE_SPRITE_06;
            break;
        case 7:
            spritePath = SOLID_TILE_SPRITE_07;
            break;
        case 8:
            spritePath = SOLID_TILE_SPRITE_08;
            break;
        case 9:
            spritePath = SOLID_TILE_SPRITE_09;
            break;
        case 10:
            spritePath = SOLID_TILE_SPRITE_10;
            break;
        case 11:
            spritePath = SOLID_TILE_SPRITE_11;
            break;
        case 12:
            spritePath = SOLID_TILE_SPRITE_12;
            break;
        case 13:
            spritePath = SOLID_TILE_SPRITE_13;
            break;
        case 14:
            spritePath = SOLID_TILE_SPRITE_14;
            break;
        case 15:
            spritePath = SOLID_TILE_FULL_BLACK;
            break;
        default:
            break;
    }
    
    auto tile = Sprite::createWithSpriteFrameName(spritePath);
    tile->setPosition(Vec2(xIndex * TILE_WIDTH, yIndex * TILE_WIDTH));
    tile->setColor(SOLID_TILE_COLOR);
    tile->setGlobalZOrder(10);
    this->addChild(tile, 0);
    
    _mapNodes.push_back(tile);
    
    //create physics body
    auto tilePBody = PhysicsBody::createBox( Size( TILE_WIDTH , TILE_WIDTH ) );
    tilePBody->setDynamic(false);
    tilePBody->setCollisionBitmask(SOLID_BITMASK);
    tilePBody->setContactTestBitmask(true);
    tile->setPhysicsBody(tilePBody);
}

void MapHandler::createOuterTile(int xIndex, int yIndex, int dir)
{
    //create tile
    auto tile = Sprite::createWithSpriteFrameName(OUTER_TILE_SPRITE_01);
    tile->setPosition(Vec2(xIndex * TILE_WIDTH, yIndex * TILE_WIDTH));
    tile->setScaleX(tile->getScaleX() * dir );
    tile->setGlobalZOrder(100);
    this->addChild(tile);
    _mapNodes.push_back(tile);
    
    //create physics body
    auto tilePBody = PhysicsBody::createBox( Size( TILE_WIDTH , TILE_WIDTH ) );
    tilePBody->setDynamic(false);
    tilePBody->setCollisionBitmask(SOLID_BITMASK);
    tilePBody->setContactTestBitmask(true);
    tile->setPhysicsBody(tilePBody);
}

void MapHandler::createClutterTile(int xIndex, int yIndex, ClutterType clutterType)
{
    ClutterTile* cTile = ClutterTile::create( clutterType );
    cTile->setPosition(Vec2( xIndex * TILE_WIDTH, yIndex * TILE_WIDTH ));
    this->addChild(cTile);
     _mapNodes.push_back(cTile);
}

void MapHandler::createClutterObject(int xIndex, int yIndex){

    ClutterObject* cObj = ClutterObject::create();
    cObj->setPosition(Vec2( xIndex * TILE_WIDTH, yIndex * TILE_WIDTH ));
    this->addChild(cObj);
    _mapNodes.push_back(cObj);
    
}

void MapHandler::createSpike(int xIndex, int yIndex, Spike::DIRECTION dir)
{
    Spike* spikeNode = Spike::create();
    spikeNode->setPosition(Vec2( xIndex * TILE_WIDTH, yIndex * TILE_WIDTH ));
    spikeNode->setDirection(dir);
    this->addChild(spikeNode);
    _mapNodes.push_back(spikeNode);
}

void MapHandler::createExplosive(int xIndex, int yIndex){

    ExplosiveCrate* eCrate = ExplosiveCrate::create();
    eCrate->setPosition(Vec2(xIndex * TILE_WIDTH, yIndex * TILE_WIDTH));
    this->addChild(eCrate);
    _movingObjects.push_back(eCrate);
    
}

void MapHandler::createSwing(int xIndex, int yIndex){

    if( _swingsSpawned >= MAX_SWING_SPAWNS )
        return;
    
    Swing* swing = Swing::create();
    swing->setPosition(Vec2( xIndex * TILE_WIDTH, yIndex * TILE_WIDTH ));
    this->addChild(swing, 10);
    _movingObjects.push_back(swing);
    _swingsSpawned++;
    
}

void MapHandler::createBat(int xIndex, int yIndex){

    if( _batsSpawned >= MAX_BAT_SPAWNS){
        return;
    }
    
    Bat* bat = Bat::create();
    bat->setPosition( Vec2(xIndex * TILE_WIDTH, yIndex * TILE_WIDTH) );
    this->addChild(bat);
    _movingObjects.push_back(bat);
    _batsSpawned++;
    
}

void MapHandler::createGraveStone(int xIndex, int yIndex){
    
    GraveStone* grvs = GraveStone::create();
    grvs->setPosition( Vec2(xIndex * TILE_WIDTH, yIndex * TILE_WIDTH) );
    this->addChild(grvs);
    _mapNodes.push_back(grvs);
    
}

void MapHandler::createFood(int xIndex, int yIndex)
{
    Food* foodNode = Food::create();
    foodNode->setPosition(Vec2( xIndex * TILE_WIDTH, yIndex * TILE_WIDTH ));
    foodNode->setGlobalZOrder(-50);
    this->addChild(foodNode);
    _mapNodes.push_back(foodNode);
}

void MapHandler::createCoin(int xIndex, int yIndex)
{
    Coin* coinNode = Coin::create();
    coinNode->setPosition(Vec2( xIndex * TILE_WIDTH, yIndex * TILE_WIDTH ));
    this->addChild(coinNode);
    _mapNodes.push_back(coinNode);
}







