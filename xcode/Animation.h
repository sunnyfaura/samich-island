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

class Sprite
{
public:
    
    string spriteName;
    Vector2 posInSS;
    
    Sprite (string ss_name, Vector2 posinss) :
    spriteName(ss_name), posInSS (posinss)
    {}
    

};

class Animation {
    private:
    string animName;
    float changeFrameRate;
    int animWidth, animHeight; //dimensions of the sprite
    Sprite* currentSprite; //currentFrame rendering in the window
    vector<Sprite*> sprites; //frames for sprite
    AABB positionRect; //position in the main window not on the spritesheet
    float totalTime;
    
    public:
    // constructor : use optional
    Animation() {}
    
    Animation(string aName, int width, int height, float changeFR)
    {
        animName = aName;
        animWidth = width;
        animHeight = height;
    }
    
    void addSprite( Sprite *newSprite )
    {
        sprites.push_back(newSprite);
        currentSprite = sprites.back();
    }
    
    void update( float dt )
    {
        totalTime += dt;
    }
    
    void setCurrentSprite( string name , float dt)
    {
        //checks if there will be not change in frames
        update ( dt );
        
        if (sprites.empty())
            return ;
        
        if (totalTime / changeFrameRate == 0)
        {
            currentSprite = findSprite(name);
        }
    }
    
    bool spriteExists( string name )
    {
        for ( size_t i = 0; i < sprites.size(); ++i )
        {
            if ( sprites[i]->spriteName == name )
            {
                return true;
            }
        }
        return false;
    }
    
    Sprite* findSprite( string name )
    {
        for ( size_t i = 0; i < sprites.size(); ++i )
        {
            if ( sprites[i]->spriteName == name )
            {
                return sprites[i];
            }
        }
        return 0;
    }
    
    //getters and setters
    void setWidth( int w ) { animWidth = w; }
    void setHeight( int h ) { animHeight = h; }
    int getWidth() { return animWidth; }
    int getHeight() { return animHeight; }
    Sprite* getCurrentSprite() { return currentSprite; }
    Vector2 getPosInSS () { return currentSprite->posInSS; }
    int getNumberOfFrames() { return (int) sprites.size(); }
    string getAnimName() { return animName; }

    void updatePosition( AABB posRect ) {
        positionRect = posRect;
    }
    
    AABB getPosition() { return positionRect; }

    
    ci::Area getSSBounds()
    {
        return Area( currentSprite->posInSS.x , currentSprite->posInSS.y,
                    currentSprite->posInSS.x + animWidth, currentSprite->posInSS.y + animHeight);
    }

};



#endif