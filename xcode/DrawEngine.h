#ifndef DRAWENGINE_HPP__
#define DRAWENGINE_HPP__

#ifdef __APPLE__
#define loadFile loadResource
#endif

#include "Headers.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"


class DrawEngine
{
    string background_fpath, spritesheet_fpath;
    
    int spritesheet_width, spritesheet_height;
    
    float total_frames, frame_rate;
    
    gl::Texture background, spritesheet;
    
    vector<Animation*> animations;
    
    ci::Rectf bounds;
    
public:
    DrawEngine() {}
    
    DrawEngine( string bg_fpath, string ss_fpath, float frame_rate ) :
    background_fpath(bg_fpath), spritesheet_fpath(ss_fpath), frame_rate(frame_rate)
    {
        total_frames = 0;
        init();
    }
    
    void update( float dt ) {
        total_frames++;
    }
    
    
    void setBackgroundPath( string bg_fpath )
    {
        background_fpath = bg_fpath;
        try {
            background = gl::Texture( loadImage( loadAsset(bg_fpath) ));
        }
        catch (Exception e) {
            background = gl::Texture( loadImage( loadFile(bg_fpath) ));
        }
    
        if (!background)
        {
            return ;
        }
    }

    void setSpritesheetPath( string ss_fpath )
    {
        spritesheet_fpath = ss_fpath;
        try {
            spritesheet = gl::Texture( loadImage( loadAsset(ss_fpath) ));
        }
        catch (Exception e) {
            spritesheet = gl::Texture( loadImage( loadFile(ss_fpath) ));
        }
        
        if (!spritesheet)
            return ;
    }

    bool animationExists( string name ) {
        for (int i = 0; i < animations.size(); ++i )
        {
            if (name == animations[i]->getAnimName())
            {
                return true;
            }
        }
        return false;
    }

    Animation* getAnimation( string name ) {
        for (int i = 0; i < animations.size(); ++i )
        {
            if (name == animations[i]->getAnimName())
            {
                return animations[i];
            }
        }
        //indeterminate
    }

    void updateBounds( ci::Rectf bnds )
    {
        bounds = bnds;
    }
    
    void addAnimations(Animation* newAnim)
    {
        animations.push_back(newAnim);
    }


    void updatePositions(string name, AABB nPos)
    {
        if ( animationExists(name) )
        {
            getAnimation(name)->updatePosition(nPos);
        }
        else
            return ;
    }

    void drawBackground() {
        if (background)
            gl::draw(background, bounds);
    }

    void drawAnimations()
    {
        if (!spritesheet)
            return ;
        
        glColor3f(1,1,1);
        for ( size_t i = 0; i < animations.size(); ++i )
        {
            gl::draw( spritesheet, animations[i]->getSSBounds(), createRectangle( animations[i]->getPosition()) );
        }
    }

    void drawAnimation( string name )
    {
        glColor3f(1,1,1);
        if ( animationExists(name) ) //if there is really an animation with that name
        {
            if ( getAnimation(name) )
                gl::draw( spritesheet, getAnimation(name)->getSSBounds(), createRectangle( getAnimation(name)->getPosition() ) );
        }
        else
            return ;
    }
    
protected:
    void init()
    {
        try {
            background = gl::Texture( loadImage( loadAsset(background_fpath) ));
        }
        catch (Exception e) {
            background = gl::Texture( loadImage( loadFile(background_fpath) ));
        }

        if (!background)
            return ;
        
        try {
            spritesheet = gl::Texture( loadImage( loadAsset(spritesheet_fpath) ));
        }
        catch (Exception e) {
            spritesheet = gl::Texture( loadImage( loadFile(spritesheet_fpath) ));
        }
        
        if (!spritesheet)
            return ;
    }
};

#endif
