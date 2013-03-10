/*
 *  Sprite.h
 *  SamichIsland
 *
 *  Created by xanthian on 2/11/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SPRITE_HPP__
#define SPRITE_HPP__

#include "BasicShapes.h"
#include "Headers.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

typedef unsigned int uint;

class Sprite : public Entity {
    /*
    entity_id : the group id for frames for the same sprite
    i.e. all frames for movement of the hero is one sprite
    
    position : position on the window
     
    virtual
        Vector2 position
        uint entity_id
    */
    
    public:
        class Frame : public Entity
        {
            /*
            frame_entity_id : entity_id for each individual frame
            posInSpriteSheet : the frames relative position on the spritesheet
            */
            public:
            Vector2 locationInSpriteSheet;
        };
    
    private:
    int sprite_width, sprite_height; //dimensions of the sprite
    Frame* currentFrame; //currentFrame rendering in the window
    vector<Frame*> frames; //frames for sprite
    
    public:
    // constructor : use optional
    Sprite ( int width, int height)
    {
        sprite_width = width;
        sprite_height = height;
    }
    
    // addFrames: adds a frame to the sprite
    void addFrame( uint frame_entity_id, Vector2 locationInSS)
    {
        Frame* f;
        f->entity_id = frame_entity_id;
        f->locationInSpriteSheet = locationInSS;
        frames.push_back(f);
        currentFrame = f;
    }
    
    //changes current frames for animations
    void changeCurrentFrame( uint frame_entity_id )
    {
        //checks if there will be not change in frames
        if( currentFrame->entity_id == frame_entity_id)
            return ;
        
        if (frames.empty())
            return ;
        
        for ( size_t i = 0; i < frames.size(); ++i )
        {
            if (frames[i]->entity_id == frame_entity_id )
                currentFrame = frames[i];
        }
        return ;
    }
    
    bool frameExists( uint frame_entity_id )
    {
        for ( size_t i = 0; i < frames.size(); ++i )
        {
            if ( frames[i]->entity_id == frame_entity_id )
            {
                return true;
            }
        }
        return false;
    }
    
    //getters and setters
    void setWidth( int w ) { sprite_width = w; }
    void setHeight( int h ) { sprite_height = h; }
    
    int getWidth() { return sprite_width; }
    int getHeight() { return sprite_height; }
    Frame* getCurrentFrame() { return currentFrame; }
    Vector2 getLocationInSS () { return currentFrame->locationInSpriteSheet; }
    int getNumberOfFrames() { return (int) frames.size(); }
    
    //finds the frame "frame_entity_id" in frame array "frames"
    Frame* findFrame( uint frame_entity_id )
    {
        for ( size_t i = 0; i < frames.size(); ++i )
        {
            if ( frames[i]->entity_id == frame_entity_id )
            {
                return frames[i];
            }
        }
        return 0;
    }
    
    //returns rectangle where sprite is on spritesheet
    Area getSpriteOnSpritesheetBounds()
    {
        AABB a;
        a.center = currentFrame->locationInSpriteSheet;
        a.width = sprite_width;
        a.height = sprite_height;
        return Area(a.center.x - a.half_width(),
                                a.center.y - a.half_height(),
                                a.center.x + a.half_width(),
                     a.center.y + a.half_height()	);
    }
    
    // returns rectangle where sprite will be pasted on window
    ci::Rectf getSpriteOnWindowBounds()
    {
        AABB a;
        a.center = position;
        a.width = sprite_width;
        a.height = sprite_height;
        return createRectangle(a);
    }
    

};



#endif