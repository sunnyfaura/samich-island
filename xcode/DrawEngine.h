/*
 *  DrawEngine.h
 *  SamichIsland
 *
 *  Created by xanthian on 2/3/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef DRAWENGINE_HPP__
#define DRAWENGINE_HPP__

#include "Headers.h"
#include "Sprite.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"

typedef unsigned int uint;

using namespace std;


class DrawEngine
{
    float frame_rate, total_time; //frameRate
    string background_mpath, ssheet_mpath;
    gl::Texture background, spritesheet;
    ci::Rectf window_bounds;
    
public:
    static DrawEngine& get();
    void setFrameRate(float fr ) { frame_rate = fr; }
    
    void setBackgroundPath ( string bg_path ) {
        background_mpath = bg_path;
        background = gl::Texture( loadImage( loadAsset(background_mpath) ));
    }
    void setSpriteSheetPath ( string ss_path ) {
        ssheet_mpath = ss_path;
        spritesheet = gl::Texture( loadImage( loadAsset(ssheet_mpath) ));
    }
    
    void setWindowBounds( ci::Rectf bnds ) { window_bounds = bnds; }
    
    DrawEngine(){}
    
    //don't use if try to upload background only
    DrawEngine( string bg_path, string ss_path, float frameRate, ci::Rectf bounds ){
        background_mpath = bg_path;
        ssheet_mpath = ss_path;
        window_bounds = bounds;
        init();
    }
    
    Sprite* findSprite( uint entity_id_query )
    {
        for ( size_t i = 0; i < sprites.size(); ++i )
        {
            if ( sprites[i]->entity_id == entity_id_query )
                return sprites[i];
        }
        return 0;
    }
    
    //checks if sprite "entity_id_query" exist
    bool spriteExists(uint entity_id_query)
    {
        for ( size_t i = 0; i < sprites.size(); ++i )
        {
            if ( sprites[i]->entity_id == entity_id_query )
                return true;
        }
        return false;
    }
    
    //entity_id : group of frames/ sprite | frame_entity_id : individual frame on sprite
    //locationInSS : location of frame in the sprite sheet
    //set frame_entity_id to equal to entity_id if only one frame
    void addFrames(uint entity_id, uint frame_entity_id, Vector2 locationInSS )
    {
        Sprite* s;
        if (spriteExists(entity_id) == true) //if sprite already exists
        {
            //adds frame "frame_entity_id" to sprite "entity_id"
            s = findSprite(entity_id);
            //s->addFrame( frame_entity_id , locationInSS );
        }
        else //if sprite "entity_id" does not exist on the Sprite array
        {
            s->entity_id = entity_id;
            s->addFrame(frame_entity_id, locationInSS);
            sprites.push_back( s );
        }
    }
    
    //entity_id : sprite whose current frame will be changed
    //frame_entity_id : the frame that will replace the current frame
    void changeFrames( uint entity_id, uint frame_entity_id, float dt)
    {
        Sprite* s;
        total_time += dt;
        
        if (spriteExists(entity_id)) //checks if sprite exists
        {
            s = findSprite(entity_id);
            if ( s->frameExists(frame_entity_id) ) //checks on the sprite if frame exists
                s->changeCurrentFrame(frame_entity_id); //replaces current frame with that frame
            else
                return ; //if frame does not exist
        }
        else //if it does not exist
        {
            return ;
        }
    }
    
    //position : the position in the window
    //entity_id: the sprite "entity_id" that will be changed
    void moveToPosition( Vector2 position, uint entity_id)
    {
        Sprite* s;
        if(spriteExists(entity_id))
        {
            s = findSprite(entity_id);
            s->position = position;
        }
        else
        {
            return ;
        }
    }
    
    void drawSprites()
    {
        // draw : draw textures here
        //gl::draw(Texture tex, Area boundsInSpriteSheet, RectInWindow);
        gl::clear( Color( 0 , 0, 0 ) );
        glColor3f(1, 1, 1); //white
        
        gl::draw(background);
        for ( size_t i = 0; i < sprites.size(); ++i )
        {
            gl::draw(spritesheet, sprites[i]->getSpriteOnSpritesheetBounds(), sprites[i]->getSpriteOnWindowBounds());
        }
        
    }
    
protected:
    vector<Sprite*> sprites;
    
    void init() {
        background = gl::Texture( loadImage( loadAsset(background_mpath) ));
        spritesheet = gl::Texture( loadImage( loadAsset(ssheet_mpath)    ));
    }
};
#endif

