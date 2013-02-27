#include "Headers.h"
#include "cinder/Camera.h"

using namespace ci;
using namespace ci::app;
using namespace std;
/*
 *  SamichIslandApp.h
 *  SamichIsland
 *
 *  Created by xanthian on 2/3/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef SAMICHISLANDAPP_HPP__
#define SAMICHISLANDAPP_HPP__
#endif

const size_t NUM_ENEMIES = 10;
const float PI = 3.141592653589793238462643383279502884197;

class SamichIslandApp : public AppBasic {
public:
	//window details
	float WIND_H, WIND_W;
	//camera
	CameraPersp mCam;
	//hero details
	Hero hero;
	float JUMP_H;
	//bullet details
	float B_RAD, B_VEL; 
	//shooting requirements
	bool leftClick;
	int bullet_counter;
	float shoot_delay;
	//lists
	vector<Bullet> dakka;
	vector<Mook> mooker;
	vector<Drop> drops;
	//platform things
	AABB platformA; //top left
	AABB platformB; //top right
	AABB platformC; //top center
	AABB platformD; //mid left
	AABB platformE; //mid right
	AABB platformF; //bottom left
	AABB platformG; //bottom right
	AABB platformH; //bottom cente
	//tubes for enemies to exit
	AABB tubeA;
	AABB tubeB;
	//others
	void prepareSettings( Settings *settings );
	void resize(ResizeEvent event);
	void setup();
	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );
	void mouseMove( MouseEvent event );
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event );
	void update();
	void draw();
};