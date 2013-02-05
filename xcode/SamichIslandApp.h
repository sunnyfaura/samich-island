#ifndef SAMICHISLANDAPP_HPP__
#define SAMICHISLANDAPP_HPP__

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

extern const size_t NUM_ENEMIES = 1;

class SamichIslandApp : public AppBasic {
public:
	Circle player;
	vector<Circle> enemies;
	void prepareSettings( Settings *settings );
	void setup();
	void keyDown( KeyEvent event );
	void mouseMove( MouseEvent event );
	void update();
	void draw();
};

#endif