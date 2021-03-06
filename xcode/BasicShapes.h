#ifndef BASICSHAPES_HPP__
#define BASICSHAPES_HPP__

#include "cinder/gl/gl.h"
#include "Vector2.h"

using namespace std;

const float PIE = 3.1416;

struct Circle {
	float radius;
	Vector2 center, velocity;
	ci::Colorf color;	
};

struct AABB {
    string name;
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

struct Tower : AABB {
    int health;
    int max_health;
    
    void recieveDamage( int decrement )
    {
        health -= decrement;
    }
    
    bool isNotDestroyed () { return health <= 0; }
    
    void initHealth( int maxhealth  )
    {
        
    }
    
    void addMaxHealth(int increment)
    {
        max_health += increment;
    }
    
    void resetHealth()
    {
        if ( isNotDestroyed() == false )
            health = max_health;
    }
};

struct Hero: AABB {
    ci::Colorf color;
    Vector2 velocity;
    int life;
	int damage; //entity numbers and stats
    int maximum_mana;
    int mana; //mana
	float dashDir;//takes note of the direction of dash
    bool isDead;
	bool moving, jumping, dashing, punching, needsGravity;
	bool on_btm_platform, on_left_platform, on_right_platform, on_top_platform; //any changes in player state
	bool jumpKey, leftKey, rightKey, dashKey; //movement details
    
    // void recieveDamage( int dmg ) {
    //     health -= dmg;
    // }
    
    // bool isAlive() { return (health > 0); }
    bool canRegenerate() { return mana <= maximum_mana; }
    bool manaNotEmpty() { return mana >= 0; }
    bool sufficientMana( int decrement ) { return mana >= decrement; }
    
    void regenerateMana ( int increment ) {
        if ( canRegenerate() == true )
            mana += increment;
        else
            mana = maximum_mana;
    }

    bool activateMana ( int decrement ) {
        if ( manaNotEmpty() == true) {
            if ( mana >= decrement) {
                mana -= decrement;
                return true;
            }
        }
        else
            mana = 0;
        return false;
        
    }
};

struct Punch: Circle {
	bool isRight;//direction of punch
};

struct Bullet: Circle {
	Vector2 direction;
};

struct Mook: Circle {
    Vector2 velocity;
    int id;
	int health;
	int max_health;
    int attack;
    bool exists;
    vector<Vector2> list;
    
    bool isAlive() { return (health > 0); }
    
	void recieveDamage(int damage_recieved) {
		health -= damage_recieved;
	}
    
    void regenerateHealth( int regen_increment )
    {
        health += regen_increment;
    }
    
    void increaseAttack( int attack_increment )
    {
        attack += attack_increment;
    }
    
    void increaseMaxHealth( int totalhealth_increment )
    {
        max_health += totalhealth_increment;
    }
    
	bool on_btm_platform, on_left_platform, on_right_platform, on_top_platform;
	float direction;
	Vector2 prev, next;
	float lerp_time;
};

struct Drop:Circle {
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

struct Int {
    int dist;
};

struct Float {
    float dist;
};

Vector2 generalLerp( float t, vector<Vector2> arr ){
    int i = 1;
    float total = 0;
    vector<Int> dists;
    for(; i < arr.size(); ++i){
        float sq_dist = ( ( arr[i-1].x - arr[i].x )*( arr[i-1].x - arr[i].x ) ) + ( ( arr[i-1].y - arr[i].y ) * ( arr[i-1].y - arr[i].y ) );
        Int temp;
        temp.dist = sqrt(sq_dist);
        total += temp.dist;
        //dists.push_back(temp);
        //console() << dist <<std::endl;
    }
    
    //float curr = 0;
    //float start = 0, end;
    vector<Float> ranges;
    Float bleh;
    bleh.dist = 0;
    ranges.push_back(bleh);
    for(i = 1; i < arr.size(); ++i){
        float sq_dist = ( ( arr[i-1].x - arr[i].x )*( arr[i-1].x - arr[i].x ) ) + ( ( arr[i-1].y - arr[i].y ) * ( arr[i-1].y - arr[i].y ) );
        Int temp;
        temp.dist = sqrt(sq_dist);
        Float bluh;
        bluh.dist = temp.dist/total + ranges.back().dist;
        ranges.push_back( bluh );
    }

    for(i = 1; i < arr.size(); ++i){
        if(t <= ranges[i].dist){
            return lerp( (t- ranges[i-1].dist) / (ranges[i].dist - ranges[i-1].dist), arr[i-1], arr[i] );
        }
    }

    return arr.back();
}

#endif