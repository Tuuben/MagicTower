//
//  TempMapHandler.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 13-02-2016.
//
//

#ifndef MapHandler_h
#define MapHandler_h

#include <stdio.h>
#include "../Globals.h"
#include "../../cocos2d/cocos/cocos2d.h"
#include "../objects/ClutterTile.h"
#include "../objects/traps/Spike.h"
#include "../candypunk/objects/Actor.h"

class MapHandler : public cocos2d::Node
{
#define TILE_WIDTH 16
#define LEVEL_DEAD_ZONE_BOTTOM 8
#define LEVEL_DEAD_ZONE_TOP 4
#define LEVEL_BLOCK_HEIGHT 30
#define LEVEL_BLOCK_WIDTH 16
#define BASE_FLOOR_HEIGHT 1
#define SOLID_TILE_COLOR cocos2d::Color3B(255, 255, 255)
#define SOLID_SPAWN_CHANCE 60 //50
#define COIN_SPAWN_CHANCE 0.2f
#define FOOD_SPAWN_CHANCE 1.5f
#define SWING_TRAP_SPAWN_CHANCE 4
#define SPIKE_SPAWN_CHANCE 10
#define EXPLOSIVE_SPAWN_CHANCE 2
#define BAT_SPAWN_CHANCE 0.25f
#define MAX_TILE_SPAWNS 240//38
#define MAX_SWING_SPAWNS 2
#define MAX_BAT_SPAWNS 4
    
#define EMPTY_TILE_ID 0
#define SOLID_TILE_ID 1
    
// MISC
#define GRASS_SPAWN_CHANCE 35
#define CLUTTER_OBJECT_SPAWN_CHANCE 35
#define BANNER_SPAWN_CHANCE 20
    
/**
* @class MapHandler
* @brief Maphandler is responsible for creating a random vertical tilemap and all clutter objects.
* Maphandler extends node and should be places at 0,0 on your scene. 
* On every update iteration the map will random a room block with the height and width of 
* (var) LEVEL_BLOCK_HEIGHT (var) LEVEL_BLOCK_WIDTH values.
* and space the tiles apart on x, y with (var) TILE_WIDTH.
* use updateMap() function to update your map. it will also remove old tiles that are off screen.
*/
public:
    
    static MapHandler* create();
    
    bool init();
    
    /**
     * @brief Removes old node's and creates a new room block.
     * @param clearPositionY Node's below this position will be cleared
     */
    void updateMap( float clearPositionY );
    
    /**
     * @brief Get the total width of the map.
     * @return Map width
     */
    float getMapWidth();
    
    /**
     * @brief Get the total height of the map.
     * @return Map height
     */
    float getMapHeight();
    
    // Return the moving objects in the map
    std::vector< Actor* > getMovingObjects() { return _movingObjects; };
    
private:
    
    /**
     * @brief Create's bottom floor on the map
     */
    void createLevelBaseFloor();
    
    /**
     * @brief Create's the outline walls on the left and right side of the map
     */
    void createLevelBlockOutline();
    
    /**
     * @brief Randomizes the map with int values of either.
     * 0 = non solid
     * 1 = solid 
     * Every room block has a 2 tile wide gap
     * @param cleanIterations Specifies how many times it should iterate through the map data and remove loose solid's
     */
    void createLevelContentData();
    
    /**
     * @brief Cleans the path for the player
     */
    void clearPathInContentData();
    
    /**
     * @brief Takes the map data and creates tiles and other map objects.
     */
    void createLevelContentObjects();
    
    /**
     * @brief Takes the map data and looks for large gaps to create larger objects
     */
    void createLargeLevelContentObjects();
    
    /**
     * @brief Returns a value depending on how many neighbours the tile has
     * @return Returns Tile value between 0-15 depending on neighbours
     */
    int getTileIndex(int top, int bottom, int left, int right);
    
    /**
     * @brief Clear old node's and cleans up the mapNodes vector
     * @param If the Node's Y position is below this position it will be removed.
     */
    void clearOld(float clearPositionY);
    
    void createSolidTile(int xIndex, int yIndex, int tileIndex);
    void createOuterTile(int xIndex, int yIndex, int dir);
    void createClutterTile(int xIndex, int yIndex, ClutterType clutterType);
    void createClutterObject(int xIndex, int yIndex);
    void createSpike(int xIndex, int yIndex, Spike::DIRECTION dir);
    void createExplosive(int xIndex, int yIndex);
    void createSwing(int xIndex, int yIndex);
    void createBat(int xIndex, int yIndex);
    void createFood(int xIndex, int yIndex);
    void createCoin(int xIndex, int yIndex);
    
    MapHandler(){}
    ~MapHandler(){}

private:
    int _rowIndex = 0;
    int _colIndex = 0;
    int _baseYIndex = 0;
    int _swingsSpawned = 0;
    int _batsSpawned = 0;
    cocos2d::Size _visibleSize;
    std::vector< Node* > _mapNodes;
    std::vector< Actor* > _movingObjects;
    std::vector< std::vector<int> > mapData;
};

#endif /* MapHandler_h */
