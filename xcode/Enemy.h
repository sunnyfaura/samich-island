/*
 *  Enemy.h
 *  SamichIsland
 *
 *  Created by xanthian on 2/11/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 *  ALMOST SAME IMPLEMENTATION AS HERO BUT AI STATES (Attack, Idle, Move)
 *  SAME DECLARATION AND INSTANTIATION OF CLASSES AND METHOD CALLS.
 */

#ifndef ENEMY_HPP__
#define ENEMY_HPP__

#include "Headers.h"
#include "Sprite.h"


using namespace ci;
using namespace ci::app;

struct EnemyState {
	bool Attack, Idle, Move;
};

class Enemy 
{
	vector<Sprite*> sprites;
	
public:
	Enemy ( string filePath, Vector2 center) 
	{
		sprites.push_back( new Sprite(filePath, center));
	}
	
	void addFrames(string filePath)
	{
		Sprite *s; 
		sprites.push_back(s = new Sprite(filePath, sprites[0]->getSpriteCenter()));
	}
	
	void drawSprite()
	{
		//temporary 
		for (size_t i = 0; i < sprites.size(); ++i)
		{
			sprites[i]->drawSprite();
		}
	}
	
	//Whether to use vector or x and y as params - it is your decision. Its the same
	void moveSprite(Vector2 velocity)
	{
		for (size_t i = 0; i < sprites.size(); ++i)
		{
			sprites[i]->moveSprite(velocity);
		}
	}
	
	void moveSprite(float vect_x = 0, float vect_y = 0)
	{
		for (size_t i = 0; i < sprites.size(); ++i)
		{
			sprites[i]->moveSprite(vect_x, vect_y);
		}
	}
	
	//gets enemy position
	Vector2 getPosition() { return sprites[0]->getSpriteCenter(); }
	
	~Enemy() {
		sprites.clear();
	}
	
};

#endif