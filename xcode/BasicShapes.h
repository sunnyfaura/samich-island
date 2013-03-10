#ifndef BASICSHAPES_HPP__
#define BASICSHAPES_HPP__

#include "cinder/gl/gl.h"
#include "Vector2.h"

const float PIE = 3.1416;

struct Circle {
	float radius;
	Vector2 center, velocity;
	ci::Colorf color;	
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
    int health;
	int damage; //entity numbers and stats
    int maximum_mana;
    int mana; //mana
	float dashDir;//takes note of the direction of dash
    bool isDead;
	bool moving, jumping, dashing, punching, needsGravity;
	bool on_btm_platform, on_left_platform, on_right_platform, on_top_platform; //any changes in player state
	bool jumpKey, leftKey, rightKey, dashKey; //movement details
    
    void recieveDamage( int dmg ) {
        health -= dmg;
    }
    
    bool isAlive() { return (health > 0); }
    bool canRegenerate() { return mana <= maximum_mana; }
    bool manaNotEmpty() { return mana >= 0; }
    
    void regenerateMana ( int increment ) {
        if ( canRegenerate() == true )
            mana += increment;
        else
            mana = maximum_mana;
    }

    void activateMana ( int decrement ) {
        if ( manaNotEmpty() == true)
            mana -= decrement;
        else
            mana = 0;
    }
};

struct Punch: Circle {
	bool isRight;//direction of punch
};

struct Bullet: Circle {
	Vector2 direction;
};

struct Mook: Circle {
	int health;
	int damage;
	void recieveDamage(int damage) {
		health -= damage;
	}
	bool on_btm_platform, on_left_platform, on_right_platform, on_top_platform;
	float direction;
};

struct Drop:Circle {
	Vector2 floor; //location where it is supposed to be dropped. directly below spot
	bool on_btm_platform, on_left_platform, on_right_platform, on_top_platform;
};

ci::Rectf createRectangle(AABB r) {
	return ci::Rectf(r.center.x - r.half_width(), 
					 r.center.y - r.half_height(), 
					 r.center.x + r.half_width(),
					 r.center.y + r.half_height()	);
};

bool circleOnCircleDetection( const Circle &c, const Circle &o) {
	Vector2 dist = o.center - c.center;
	float sumRadii = o.radius + c.radius;
	
	if ( dist.sqMag() < sumRadii * sumRadii ) {
		return true;
	}
	
	return false;
};

bool testSatAxis(Vector2 axis, Vector2 d, AABB a, AABB b) {
	const float ra = abs(a.u() * axis) + abs(a.v() * axis);
	const float rb = abs(b.u() * axis) + abs(b.v() * axis);
	return abs(axis * d) <= ra + rb;
}

bool satAABB(AABB a, AABB b) {
	Vector2 d = b.center - a.center;
	return testSatAxis(d, d, a, b) && 
	testSatAxis(a.u(), d, a, b) && testSatAxis(a.v(), d, a, b) &&
	testSatAxis(b.u(), d, a, b) && testSatAxis(b.v(), d, a, b);
}

bool satCircleAABB(Circle c, AABB b) {
	AABB a;
	a.center = c.center;
	a.width = c.radius;
	a.height = c.radius;
	return satAABB(a,b);
}

Vector2 lerp( float t, const Vector2& prev, const Vector2& next ) { //rhs thing
	return prev + ( next - prev ) * t;
}

float func(float x) { //ease out function
	return ( sin( PIE * x / 4 ) + 1 ) / 2;
}

#endif