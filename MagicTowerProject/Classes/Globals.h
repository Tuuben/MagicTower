//
//  Globals.hpp
//  MagicTowerProject
//
//  Created by Tobias Helsing on 13-02-2016.
//
//

#ifndef Globals_hpp
#define Globals_hpp

#include <stdio.h>


/*==== < COLLISION BITMASK'S > ====*/
#define PLAYER_BITMASK 0x000001
#define SOLID_BITMASK 0x000002
#define FOOD_BITMASK 0x000003
#define COIN_BITMASK 0x000004
#define OBSTACLE_BITMASK 0x000005
#define CLUTTER_OBJECT_BITMASK 0x000006
#define SOILD_IGNORE_BITMASK_CATEGORY -50

/*==== < PLIST'S > ====*/
#define TILEMAP_PLIST_PATH "textures/tilemap.plist"
#define ACTOR_PLIST_PATH "textures/actors.plist"
#define SPRITEMAP_PLIST_PATH "textures/spritemap.plist"

/*==== < ACTOR SPRITES > ====*/
#define BAT_01 "bat_01.png"
#define BAT_02 "bat_02.png"
#define BAT_03 "bat_03.png"
#define BAT_04 "bat_04.png"

/*==== < PARTICLE SPRITES > ====*/
#define PARTICLE_CIRCLE_01 "particle_01.png"
#define PARTICLE_DUST_01 "particle_dust_01.png"
#define PARTICLE_DUST_02 "particle_dust_02.png"
#define PARTICLE_DUST_03 "particle_dust_03.png"
#define PARTICLE_DUST_04 "particle_dust_04.png"
#define PARTICLE_SQUARE_01 "particle_square_01.png"
#define PARTICLE_EXPLOSION_01 "explosion_cloud_01.png"
#define PARTICLE_EXPLOSION_02 "explosion_cloud_02.png"
#define PARTICLE_EXPLOSION_03 "explosion_cloud_03.png"
#define PARTICLE_EXPLOSION_04 "explosion_cloud_04.png"
#define PARTICLE_CLUTTER_01 "clutter_debri_01.png"
#define PARTICLE_CLUTTER_02 "clutter_debri_02.png"
#define PARTICLE_CLUTTER_03 "clutter_debri_03.png"

/*==== < OBJECT SPRITES > ====*/
#define FOOD_IDLE_01 "food_01.png"
#define FOOD_IDLE_02 "food_02.png"
#define FOOD_IDLE_03 "food_03.png"
#define FOOD_IDLE_04 "food_04.png"
#define FOOD_PICK_UP_01 "food_pick_01.png"
#define FOOD_PICK_UP_02 "food_pick_02.png"
#define FOOD_PICK_UP_03 "food_pick_03.png"
#define FOOD_PICK_UP_04 "food_pick_04.png"
#define FOOD_PICK_UP_05 "food_pick_05.png"
#define FOOD_PICK_UP_06 "food_pick_06.png"
#define FOOD_PICK_UP_07 "food_pick_07.png"
#define COIN_IDLE_01 "coin_idle_01.png"
#define COIN_IDLE_02 "coin_idle_02.png"
#define COIN_IDLE_03 "coin_idle_03.png"
#define COIN_IDLE_04 "coin_idle_04.png"
#define COIN_IDLE_05 "coin_idle_05.png"
#define COIN_IDLE_06 "coin_idle_06.png"
#define COIN_PICK_UP_01 "coin_pickup_01.png"
#define COIN_PICK_UP_02 "coin_pickup_02.png"
#define COIN_PICK_UP_03 "coin_pickup_03.png"
#define COIN_PICK_UP_04 "coin_pickup_04.png"
#define COIN_PICK_UP_05 "coin_pickup_05.png"
#define COIN_PICK_UP_06 "coin_pickup_06.png"
#define COIN_PICK_UP_07 "coin_pickup_07.png"
#define SPIKE_SPRITE_01 "spike.png"
#define SWING_BASE_01 "chain_base.png"
#define SWING_CHAIN_01 "chain_link.png"
#define SWING_MACE_01 "chain_mace.png"
#define GRIM_REAPER_GRAVESTONE_01 "grim_gravestone_01.png"
#define EXPLOSIVE_CRATE_01 "explosive_crate.png"

/*==== < MISC SPRITES > ====*/
#define GRASS_SPRITE_01 "grass_clutter_01.png"
#define VASE_SPRITE_01 "vase.png"
#define CRATE_SPRITE_01 "crate.png"
#define CANDLE_STICK_SPRITE_01 "candle.png"
#define BANNER_01 "banner_01.png"
#define BANNER_02 "banner_02.png"
#define BANNER_03 "banner_03.png"
#define BANNER_04 "banner_04.png"
#define BANNER_05 "banner_05.png"
#define BANNER_06 "banner_06.png"
#define BANNER_07 "banner_07.png"
#define BANNER_08 "banner_08.png"
#define DITHERING_TILE "dithering.png"

/*==== < TILEMAP SPRITES > ====*/
#define SOLID_TILE_SPRITE_00 "solidTile.png"
#define SOLID_TILE_SPRITE_01 "solidTile_06.png"
#define SOLID_TILE_SPRITE_02 "solidTile_03.png"
#define SOLID_TILE_SPRITE_03 "solidTile_11.png"
#define SOLID_TILE_SPRITE_04 "solidTile_04.png"
#define SOLID_TILE_SPRITE_05 "solidTile_05.png"
#define SOLID_TILE_SPRITE_06 "solidTile_09.png"
#define SOLID_TILE_SPRITE_07 "solidTile_10.png"
#define SOLID_TILE_SPRITE_08 "solidTile_01.png"
#define SOLID_TILE_SPRITE_09 "solidTile_13.png"
#define SOLID_TILE_SPRITE_10 "solidTile_02.png"
#define SOLID_TILE_SPRITE_11 "solidTile_12.png"
#define SOLID_TILE_SPRITE_12 "solidTile_07.png"
#define SOLID_TILE_SPRITE_13 "solidTile_14.png"
#define SOLID_TILE_SPRITE_14 "solidTile_08.png"
#define SOLID_TILE_SPRITE_15 "solidTile.png"
#define SOLID_TILE_FULL_BLACK "solid_full.png"
#define OUTER_TILE_SPRITE_01 "outerTile.png"
#define BACKGROUND_TILE_01 "background_tile_01.png"
#define BACKGROUND_TILE_02 "background_tile_01.png"
#define BACKGROUND_TILE_03 "background_tile_01.png"
#define BACKGROUND_TILE_04 "background_tile_01.png"

/*==== < COLORS > ====*/
#define FOOD_COLOR cocos2d::Color3B( 0, 200, 255 )
#define COIN_COLOR cocos2d::Color3B( 0, 0, 0 )
#define OBSTACLE_COLOR cocos2d::Color3B(255, 0, 0)

/*==== < AUDIO > ====*/
#define SFX_HIT_01 "audio/hit_01.wav"
#define SFX_HIT_02 "audio/hit_02.wav"
#define SFX_PICK_UP_01 "audio/pickup_01.wav"
#define SFX_PICK_UP_02 "audio/pickup_02.wav"
#define SFX_PICK_UP_03 "audio/pickup_03.wav"
#define SFX_PICK_UP_04 "audio/pickup_04.wav"
#define SFX_PICK_UP_05 "audio/pickup_05.wav"
#define SFX_JUMP_01 "audio/jump_01.wav"



class Globals {

public:
    
};




#endif /* Globals_h */
