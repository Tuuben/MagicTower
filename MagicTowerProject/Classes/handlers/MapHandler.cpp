//
//  MapHandler.cpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 13-02-2016.
//
//

#include "MapHandler.h"
#include "../objects/traps/Swing.h"
#include "../objects/enemys/Bat.h"

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
    
    /* === < REMOVE OLD > ===
    1. remove old tiles and walls
    */
    clearOld(clearPositionY);
    
    /* === < BLOCK OUTLINE > ===
     1. creates the walls at screen border
     */
    createLevelBlockOutline();
    
     /* === < LEVEL DATA > ===
     1. randomizes map data 
     2. cleans lose tiles the number of times specified
     */
    int cleanupItr = 5;
    createLevelContentData( cleanupItr );
    
     /* === < LEVEL OBJECTS > ===
     1. uses map data and creates tiles
     2. creates other game objects
     */
    createLevelContentObjects();
    
    /* === < LARGE LEVEL OBJECTS > ===
     1. uses map data and creates large objects
     */
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
    for(int i = 1; i < LEVEL_BLOCK_WIDTH; i++)
    {
        createSolidTile(i, BASE_FLOOR_HEIGHT, 0);
    }
}

void MapHandler::createLevelBlockOutline()
{
    for(int i = _baseYIndex; i < _baseYIndex + LEVEL_BLOCK_HEIGHT; i++)
    {
        createOuterTile( 0, i, -1);
        createOuterTile( LEVEL_BLOCK_WIDTH, i, 1);
    }
    _baseYIndex += LEVEL_BLOCK_HEIGHT;
    
}

void MapHandler::createLevelContentData(int cleanIterations )
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
    int gap01 = (LEVEL_BLOCK_WIDTH - 2) * CCRANDOM_0_1() + 2;
    int gap02 = gap01 + 1;
    int numObjectsSpawned = 0;
    for(int y = 2; y < LEVEL_BLOCK_HEIGHT; y++)
    {
        //chance to change the gap
        if( y == LEVEL_BLOCK_HEIGHT / 2 )
        {
            /*float gapChangeChance = 100 * CCRANDOM_0_1();
            if(gapChangeChance < 50)
            {
                int gap01 = (LEVEL_BLOCK_WIDTH - 2) * CCRANDOM_0_1() + 1;
                int gap02 = gap01 + 1;
            }*/
        }
        
        //for(int x = 1; x < LEVEL_BLOCK_WIDTH; x++)
       // {
            int sourceX = (int)(CCRANDOM_0_1() * LEVEL_BLOCK_WIDTH - 2) + 2;
            float rand = 100 * CCRANDOM_0_1();
            
            if(rand < SOLID_SPAWN_CHANCE && numObjectsSpawned < MAX_TILE_SPAWNS)
            {
                mapData[sourceX][y] = SOLID_TILE_ID;
                numObjectsSpawned++;
            
                int clusterWidth = (5 * CCRANDOM_0_1() + 2);
                int clusterHeight = 4 * CCRANDOM_0_1() + 1;
                
                for(int cX = sourceX - (clusterWidth / 2); cX < sourceX + (clusterWidth / 2); cX++)
                {
                    
                    for( int cY = y; cY < y + clusterHeight; cY++ ){
                        
                        int spawnX = clampf(cX, 0, LEVEL_BLOCK_WIDTH - 2);
                        int spawnY = clampf(cY, 0, LEVEL_BLOCK_HEIGHT - 1);//clampf(y + cY, 0, LEVEL_BLOCK_HEIGHT - 1);
                    
                        if(cX != gap01 || cX != gap02){
                            mapData[spawnX][spawnY] = 1;
                            numObjectsSpawned++;
                        }
                        else{
                            mapData[spawnX][spawnY] = 0;
                        }
                        
                    }
                    
                }
            }
        
    }
    
    //clean lose tiles
    for(int i = 0; i < cleanIterations; i++)
    {
        for(int y = 0; y < LEVEL_BLOCK_HEIGHT; y++ )
        {
            for(int x = 0; x < LEVEL_BLOCK_WIDTH; x++)
            {
                int top = ( y == LEVEL_BLOCK_HEIGHT - 1 ) ? 0 : y + 1;
                int bottom = ( y == 0 ) ? 0 : y - 1;
                int right = (x == LEVEL_BLOCK_WIDTH - 1) ? 0 : x + 1;
                int left = (x == 0) ? 0 : x - 1;
                
                int tileValue = 0;
                
                tileValue += mapData[x][top];
                tileValue += mapData[x][bottom];
                tileValue += mapData[left][y];
                tileValue += mapData[right][y];

                //clean tiles
                if(tileValue == 0) {
                    mapData[x][y] = EMPTY_TILE_ID;
                }
                if(tileValue >= 3) {
                    mapData[x][y] = SOLID_TILE_ID;
                }
                else if(tileValue < 3 && tileValue > 0)
                {
                    if( (100 * CCRANDOM_0_1()) < 10)
                    {
                        mapData[x][y] = EMPTY_TILE_ID;
                    }
                }
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
                if(tileIndex < 15)
                {
                    createSolidTile(x, y + _baseYIndex, tileIndex);
                }
                
                // create swing traps
                if( (tileIndex < 15) && ((100 * CCRANDOM_0_1()) < SWING_TRAP_SPAWN_CHANCE) && (x > 4) && (x < LEVEL_BLOCK_WIDTH - 4) ){
                
                    createSwing(x, y + _baseYIndex);
                    
                }
            }
            
            if(mapData[x][y] == EMPTY_TILE_ID)
            {
                int spotFilled = 0;
                
                //create spikes
                if(tileIndex < 15)
                {
                    float chance = 100 * CCRANDOM_0_1();
                    float spawnChance = 10;
                    if(chance < spawnChance)
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
                    
                }
                
                //create grass clutter
                if( (tileIndex == 4 || tileIndex == 5 || tileIndex == 6 || tileIndex == 12) && spotFilled < 1)
                {
                    float chance = 100 * CCRANDOM_0_1();
                    float spawnChance = 70;
                    if(chance < spawnChance)
                    {
                        createClutterTile(x, y + _baseYIndex, ClutterType::GRASS );
                    }
                }
                
                //create background clutter
                if(tileIndex <= 15 && spotFilled < 1)
                {
                    float chance = 100 * CCRANDOM_0_1();
                    if(chance < CLUTTER_TILE_SPAWN_CHANCE)
                    {
                        CCLOG("CREATE");
                        createClutterTile(x, y + _baseYIndex, ClutterType::BACKGROUND );
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
                    
                    if( (100 * CCRANDOM_0_1()) < BAT_SPAWN_CHANCE) {
                        CCLOG("BAT");
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
                    CCLOG("SPAWN SWINGUGURURU");
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

void MapHandler::createSpike(int xIndex, int yIndex, Spike::DIRECTION dir)
{
    Spike* spikeNode = Spike::create();
    spikeNode->setPosition(Vec2( xIndex * TILE_WIDTH, yIndex * TILE_WIDTH ));
    spikeNode->setDirection(dir);
    this->addChild(spikeNode);
    _mapNodes.push_back(spikeNode);
}

void MapHandler::createSwing(int xIndex, int yIndex){

    if( _swingsSpawned >= MAX_SWING_SPAWNS )
        return;
    
    Swing* swing = Swing::create();
    swing->setPosition(Vec2( xIndex * TILE_WIDTH, yIndex * TILE_WIDTH ));
    this->addChild(swing, 10);
    _mapNodes.push_back(swing);
    _swingsSpawned++;
    
}

void MapHandler::createBat(int xIndex, int yIndex){

    if( _batsSpawned >= MAX_BAT_SPAWNS){
        return;
    }
    
    Bat* bat = Bat::create();
    bat->setPosition( Vec2(xIndex * TILE_WIDTH, yIndex * TILE_WIDTH) );
    this->addChild(bat);
    _mapNodes.push_back(bat);
    _batsSpawned++;
    
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







