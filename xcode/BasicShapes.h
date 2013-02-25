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

struct Circle
{
	float radius;
	Vector2 center, velocity;
	ci::Colorf color;
	
	float mass() const {
		return radius * radius * 3.141592653589793238462643383279502884197 + 1;
	}	
};

struct AABB {
	Vector2 center;
	float width;
	float height;
	float half_width(){
		return width/2;
	}
	float half_height(){
		return height/2;
	}
	Vector2 u(){
		return Vector2( half_width() , 0);
	}
	Vector2 v(){
		return Vector2( 0 , half_height() );
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

ci::Rectf createRectangle(AABB r)
{
	return ci::Rectf(r.center.x - r.half_width(), 
		r.center.y - r.half_height(), 
		r.center.x + r.half_width(),
		r.center.y + r.half_height()	);
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

bool testSatAxis(Vector2 axis, Vector2 d, AABB a, AABB b){
	const float ra = abs(a.u() * axis) + abs(a.v() * axis);
	const float rb = abs(b.u() * axis) + abs(b.v() * axis);
	return abs(axis * d) <= ra + rb;
}

bool satAABB(AABB a, AABB b){
	Vector2 d = b.center - a.center;
	return testSatAxis(d, d, a, b) && 
	testSatAxis(a.u(), d, a, b) && testSatAxis(a.v(), d, a, b) &&
	testSatAxis(b.u(), d, a, b) && testSatAxis(b.v(), d, a, b);
}

bool satCircleAABB(Circle c, AABB b){
	AABB a;
	a.center = c.center;
	a.width = c.radius;
	a.height = c.radius;
	return satAABB(a,b);
}

#endif                                                    