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

#include "Headers.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Sprite {
private:
	string filePath; //filepath
	Vector2 center, //location of sprite on program
	spriteSheetLoc; //location of desired sprite on sprite sheet
	float width, height; //width and height of texture
	gl::Texture tex; //texture loaded into memory

	public:
		Sprite () {}
		
		//not a sprite sheet
		Sprite (string filePath, Vector2 center): 
		center(center), filePath(filePath)
		{ 
			tex = gl::Texture( loadImage ( loadResource (filePath) ) );
			width = tex.getWidth();
			height = tex.getHeight();
		}
		
		//sprite sheet - may not be implemented yet.
		Sprite (string filePath, float length, float width, Vector2 spriteSheetLoc = Vector2(100,100) ):
		spriteSheetLoc(spriteSheetLoc), width(width), height(height)
		{
			tex = gl::Texture( loadImage ( loadResource (filePath) ) );
		}
	
		//self-explanatory
		void drawSprite()
		{
			/*gl::draw(tex, <--- SPRITE SHEET IMPLEMENTATION
			  Area( createRectangle(spriteLocation,width,height)), 
		      createRectangle(Vector2(center),width,height))  ; */
			
			glEnable( GL_BLEND );
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			//check namespace gl on cinder
			gl::draw(tex, 
					 Area( tex.getBounds()), 
					 createRectangle(Vector2(center),width,height))  ;
		}
		
		//move sprite to a new position
		void moveSprite( Vector2 velocity )
		{
			center += velocity;
		}
		
		//move sprite via increment
		void moveSprite(float vect_x = 0, float vect_y = 0)
		{
			center += Vector2(vect_x,vect_y);
		}
		
		//getters
		Vector2 getSpriteCenter() { return center; }
		float getWidth() { return width; }
		float getHeight() { return height; }
		gl::Texture getTexture() { return tex; }
		Vector2 getSpriteSheetLocation() { return spriteSheetLoc; }
		string getFilePath() { return filePath;}
		
		//destructor
		~Sprite() {
			tex = gl::Texture();
		}
};



#endif