/*
 *  BasicShapes.h
 *  SamichIsland
 *
 *  Created by xanthian on 2/2/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 *  Add All Basic Shape Creations Here
 *  For trial basis
 *
 */


#ifndef BASICSHAPES_HPP__
#define BASICSHAPES_HPP__

#include "Headers.h"

/**
 CONTAINER FOR CIRCLES
 **/
struct Circle
{
	float radius;
	Vector2 center, velocity;
	ci::Colorf color;
	
	float mass() const {
		return radius * radius * 3.141592653589793238462643383279502884197 + 1;
	}	
};

struct Hero: Circle {
	bool moving, jumping; //any changes in player state
	bool jumpKey, leftKey, rightKey; //movement details
};

struct Bullet: Circle {
	Vector2 direction;
};

struct Mook: Circle {
	
};

#endif