/*
 *  DrawEngine.h
 *  SamichIsland
 *
 *  Created by xanthian on 2/3/13.
 *  Copyright 2013 __bearStuffs__. All rights reserved.
 *
 *  how to use: use DrawEngine::get() singleton :)
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

enum TextAlign
{
  TEXT_CENTER,
  TEXT_LEFT
};

class DrawEngine
{
    float frame_rate, total_time; //frameRate
    string background_mpath, ssheet_mpath, title_mpath;
    gl::Texture background, spritesheet, title;
    int ss_width, ss_height;
    ci::Rectf window_bounds;
    
public:
    static DrawEngine& get();
    void setFrameRate(float fr ) { frame_rate = fr; }
    
    void setBackgroundPath ( string bg_path ) {
        background_mpath = bg_path;
        
        try {
            background = gl::Texture( loadImage( loadResource(background_mpath) ));
        } catch (Exception e) {
            background = gl::Texture( loadImage( loadAsset(background_mpath) ));
        }
        
    }
    void setSpriteSheetPath ( string ss_path ) {
        ssheet_mpath = ss_path;        
        try {
            spritesheet = gl::Texture( loadImage( loadResource(ssheet_mpath) ));
        } catch (Exception e) {
            spritesheet = gl::Texture( loadImage( loadAsset(ssheet_mpath) ));
        }
        
        if ( spritesheet )
        {
            
        }
        else
            return ;
        
    }
    void setTitlePath ( string t_path ) {
        title_mpath = t_path;        
        try {
            title = gl::Texture( loadImage( loadResource(title_mpath) ));
        } catch (Exception e) {
            title = gl::Texture( loadImage( loadAsset(title_mpath) ));
        }
        
        if ( title )
        {
            
        }
        else
            return ;    }
    
    void setWindowBounds( ci::Rectf bnds ) { window_bounds = bnds; }
    
    DrawEngine(){}
    
    //don't use if try to upload background only
    DrawEngine( string bg_path, string ss_path, string t_path, float frameRate, ci::Rectf bounds ){
        background_mpath = bg_path;
        ssheet_mpath = ss_path;
		title_mpath = t_path;
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
    void addFrames(uint entity_id, uint frame_entity_id, Vector2 locationInSS, Vector2 position, int width, int height )
    {
        Sprite* s;
        if (spriteExists(entity_id) == true) //if sprite already exists
        {
            //adds frame "frame_entity_id" to sprite "entity_id"
            s = findSprite(entity_id);
            s->addFrame( frame_entity_id , locationInSS );
        }
        else //if sprite "entity_id" does not exist on the Sprite array
        {
            s = new Sprite();
            s->position = position;
            s->setHeight(height);
            s->setWidth(width);
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
        glColor3f(1, 1, 1); 
        
        if ( background )
            gl::draw(background, window_bounds );
        
        if ( spritesheet && background_mpath == "bg.png")
        {
            for ( size_t i = 0; i < sprites.size(); ++i )
            {
                gl::draw(spritesheet, sprites[i]->getSpriteOnSpritesheetBounds(), sprites[i]->getSpriteOnWindowBounds());
            }
        }
    }
	
	void drawTitle()
    {
        // draw : draw textures here
        //gl::draw(Texture tex, Area boundsInSpriteSheet, RectInWindow);
        gl::clear( Color( 0 , 0, 0 ) );
        glColor3f(1, 1, 1); //white
        
        if ( background )
            gl::draw(title, window_bounds );
		
    }

    //str: the string to be drawn. must be lexical casted.
    //pos: position in center
    //color: color of text font: font
    void drawString( const string &str, const Vector2 &pos, const Color &color, const Font &font, TextAlign textState )
    {
        if ( textState == TEXT_CENTER)
            gl::drawStringCentered(str, pos.toV(), color, font);
        else if ( textState == TEXT_LEFT)
            gl::drawString(str, pos.toV(), color, font);
    }
    
    // c: circle to be drawn, color can be empty or not
    void drawCircle( const Circle &c, const Color color )
    {
        if ( !color )
            glColor3f(c.color.r ,c.color.g, c.color.b);
        else
            glColor3f(color.r ,color.g, color.b);
        gl::drawSolidCircle(c.center.toV(),c.radius, 0);
    }
    
    // a: aabb that will be drawn in the window
    // color: color of the rectangle created
    void drawRectangle( const AABB &a, const Color color)
    {
        glColor3f(color.r,color.g,color.b);
        gl::drawSolidRect(createRectangle(a));
    }
    
protected:
    vector<Sprite*> sprites;
    
    void init() {
        try {
            background = gl::Texture( loadImage( loadAsset(background_mpath) ));
            spritesheet = gl::Texture( loadImage( loadAsset(ssheet_mpath)    ));
        }
        catch (Exception e) {
            background = gl::Texture( loadImage( loadResource(background_mpath) ));
            //spritesheet = gl::Texture( loadImage( loadResource(ssheet_mpath)    ));
        }

        if (spritesheet)
        {
            ss_width = spritesheet.getWidth();
            ss_height = spritesheet.getHeight();
        }
    }
};
#endif

