#include "Headers.h"

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

const size_t NUM_ENEMIES = 10;

class SamichIslandApp : public AppBasic {
public:
	Circle player;
	Circle bullets[8];
	vector<Circle> enemies;
	float WIND_H, WIND_W; //window details
	bool moving, jumping; //any changes in player state
	bool jumpKey, leftKey, rightKey; //movement details
	bool leftClick; float bulletSpeed, angle; Vector2 scale; //shot details
	void prepareSettings( Settings *settings );
	void setup();
	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event );
	void update();
	void draw();
};

#endif