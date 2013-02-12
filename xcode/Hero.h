/*
 *  Hero.h
 *  SamichIsland
 *
 *  Created by xanthian on 2/11/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *  
 *  USES AN ARRAY OF SPRITES FOR ANIMATIONS AND STUFF
 *  FOR HOLD DIFFERENT INPUT STATES
 *
 *  Non-sprite sheet. Sprite sheets will be implemented later. 
 *  To use hero class.
 *  declare as pointer, Hero *hero, 
 *  use constructor hero = new Hero()
 *  
 *  call member functions
 *  hero->memberFunction(parameter)
 *  
 */

#ifndef HERO_HPP__
#define HERO_HPP__

#include "Headers.h"
#include "Sprite.h"

//using namespace ci;
//using namespace ci::app;
using namespace std;


//I think hmm. I think that arrays of sprites are more appropriate. For animation purposes. 

//declare here locations for different sprites in sprite sheets. 

const size_t FRAMES = 4;

struct InputState {
	bool Jump, Left, Right, Punch, Shoot;
};

class Hero {	
	vector<Sprite*> sprites;
	
public:
	//for default frame
	Hero(string filePath, Vector2 center)
	{	
		sprites.push_back( new Sprite(filePath, center));
	}
	
	//for additional frames
	void addFrames(string filePath)
	{
		Sprite *s; 
		sprites.push_back(s = new Sprite(filePath, sprites[0]->getSpriteCenter()));
	}
	
	
	void keyDown( KeyEvent event )
	{
		//input states
	}
	
	void keyUp ( KeyEvent event )
	{
		//de-input states
	}
	
	void mouseDown ( MouseEvent event )
	{
		//shooting here
	}
	
	//self-explanatory - implementation inside temporary without consideration for input states
	void drawSprite()
	{
		for (size_t i = 0; i < sprites.size(); ++i)
		{
			sprites[i]->drawSprite();
		}
	}
	
	//same in sprite
	void moveSprite(Vector2 newPos)
	{
		for (size_t i = 0; i < sprites.size(); ++i)
		{
			sprites[i]->moveSprite(newPos);
		}
	}
	
	//same in sprite
	void moveSprite(float vect_x, float vect_y)
	{
		for (size_t i = 0; i < sprites.size(); ++i)
		{
			sprites[i]->moveSprite(vect_x, vect_y);
		}
	}
	
	//gets position of hero
	Vector2 getPosition() { return sprites[0]->getSpriteCenter(); }
	
	//destructor
	~Hero()
	{
		sprites.clear();
	}
};

#endif