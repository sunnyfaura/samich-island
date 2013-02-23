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
	int damage; //entity numbers and stats
	float dashDir;//takes note of the direction of dash
	bool moving, jumping, dashing; //any changes in player state
	bool jumpKey, leftKey, rightKey, dashKey; //movement details
};

struct Bullet: Circle {
	Vector2 direction;
};

struct Mook: Circle {
	int health;
	int damage;
	bool attack, defend;
	void recieveDamage(int damage)
	{
		health -= damage;
	}
};

struct Drop:Circle {
	Vector2 floor; //location where it is supposed to be dropped. directly below spot
};

ci::Rectf createRectangle(Vector2 center, float width, float height)
{
	float hw = width/2, hh = height/2;
	return ci::Rectf(center.x-hw, center.y-hh, center.x+hw, center.y+hh);
};

bool circleOnCircleDetection( const Circle &c, const Circle &o)
{
	Vector2 dist = o.center - c.center;
	float sumRadii = o.radius + c.radius;
	
	if ( dist.sqMag() < sumRadii * sumRadii ) {
		return true;
	}
	
	return false;
};

#endif