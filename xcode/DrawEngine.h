/*
 *  Sprite.h
 *  SamichIsland
 *
 *  Created by xanthian on 2/3/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#include "Headers.h"

#ifndef SPRITE_HPP__
#define SPRITE_HPP__

using namespace std;

/**
 DrawEngine class
 
 **/


class DrawEngine
{	
	public:
		struct Sprite {
			ci::Rectf spriteBounds;
			string mPath, name;
		};
	
		//constructor
		SpriteEngine(int width = 640, int height = 480);
	
		//destructor
		~SpriteEngine();
	
		void addSprite(string mPath, Rectf bounds, string name); 
		//relative to the Resources path, adds to vector sprites
	
		void duplicateSprites( string name );
		//replicates sprite for multiple sprite rendering in games
	
		void drawSprites();
		//self explanatory
		
		void moveSprites(Vector2 delta);
		//move sprites
	
		void putSpriteToPosition( Vector2 post, string name );
		//puts a sprite name to a position
	
		void eraseSprite(string name); 
		//erases a sprite according to name
	
		void deleteSprites();
		//delete all sprites
	
	protected:
		int screenWidth, screenHeight;
		vector<Sprite> sprites;
	
	private:
		Sprite findSprite(string name);
};

/**
 CREATE new .cpp - DrawEngine.cpp
 use #include "DrawEngine.h" put on DE.cpp or Headers.h
 
 **/

#endif
