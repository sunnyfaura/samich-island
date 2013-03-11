#include "Headers.h"
#include "cinder/Camera.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "ciUI.h"

/**
 Credits for Menu GUI go completely to
 rezaali
 https://github.com/rezaali/ciUI
 **/

using namespace ci;
using namespace ci::app;
using namespace std;

#ifndef SAMICHISLANDAPP_HPP__
#define SAMICHISLANDAPP_HPP__

const size_t NUM_ENEMIES = 10;
const float PI = 3.141592653589793238462643383279502884197;

DrawEngine& DrawEngine::get() {
    static DrawEngine dg;
    return dg;
}

class SamichIslandApp : public AppBasic {
public:
//state management
	State appState;
	int count;
    int timeout;
//window details
	float WIND_H, WIND_W;
//camera
	CameraPersp mCam;
//hero details
	Hero hero;
	float JUMP_H;
	Punch punch;
//bullet details
	float B_RAD, B_VEL; 
//shooting requirements
	bool leftClick;
	int bullet_counter;
	float shoot_delay;
//lists
	vector<Bullet> dakka;
	vector<Mook> cannon_fodder;
	int MAX_MOOKS;
	vector<Drop> drops;
//platform things
	AABB bottom_platform, left_platform, right_platform, top_platform;	
	AABB tubeA; //tubes for exit
	AABB tubeB;
	AABB portal; //portal from hell
    AABB towerguard1; //protect tower2
    AABB towerguard2; //protect tower1
//tower
    Tower tower1; //tower on top left
    Tower tower2; //tower on top right
//for menu
    ciUICanvas *menuGUI;
//audio
	audio::SourceRef intro_song;
//others
	void prepareSettings( Settings *settings );
	void resize(ResizeEvent event);
	void setup();
	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );
	void mouseMove( MouseEvent event );
	void mouseDown( MouseEvent event );
	void mouseUp( MouseEvent event );
    void guiEvent( ciUIEvent *event);
	void update();
	void draw();
    void shutdown();
};

#endif