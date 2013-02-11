#include "Headers.h"
#include "SamichIslandApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

float game_time, shoot_time;
Vector2 mouse;

void SamichIslandApp::prepareSettings( Settings *settings ){
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 60.0f );
}

void SamichIslandApp::setup()
{	
	game_time = 0, shoot_time = 0;
	//window bluh
	WIND_H = this->getWindowHeight();
	WIND_W = this->getWindowWidth();
	//hero initialization
	hero.radius = 20;
	hero.center = Vector2(WIND_W/2, WIND_H - hero.radius);
	hero.color = Colorf(1.0,1.0,1.0);
	hero.velocity.x = 12.0;
	hero.velocity.y = 12.0;
	hero.moving = hero.jumping = hero.jumpKey = hero.leftKey = hero.rightKey = false;
	//bullet initialization
	B_RAD = 7.00; B_VEL = 7.00;
	//other bullet bluh
	leftClick = false;
	bullet_counter = 50;
	shoot_delay = 500;
}

void SamichIslandApp::keyDown( KeyEvent event ) {
	int code = event.getCode();
	if (code==KeyEvent::KEY_a) {
		hero.leftKey = true;
	}

	if (code==KeyEvent::KEY_d) {
		hero.rightKey = true;
	}

	if (code==KeyEvent::KEY_SPACE) {
		hero.jumpKey = true;
	}
}

void SamichIslandApp::keyUp( KeyEvent event ) {
	int code = event.getCode();
	if (code==KeyEvent::KEY_a) {
		hero.leftKey = false;
	}

	if (code==KeyEvent::KEY_d) {
		hero.rightKey = false;
	}

	if (code==KeyEvent::KEY_SPACE) {
		hero.jumpKey = false;
	}
}

void SamichIslandApp::mouseMove( MouseEvent event ) {
	mouse.x = event.getX();
	mouse.y = event.getY();
}

void SamichIslandApp::mouseDown( MouseEvent event ) {
	leftClick = true;
}

void SamichIslandApp::mouseUp( MouseEvent event ) {
	leftClick = false;
}

void SamichIslandApp::update() {	
	//dakka dakka dakka!
	game_time = ci::app::getElapsedSeconds() * 1000;
	if(leftClick == true && game_time - shoot_time > shoot_delay){
		shoot_time = game_time;
		Bullet temp;
		temp.center = hero.center;
		temp.radius = B_RAD;
		temp.velocity.x = temp.velocity.y = B_VEL;

		console() << "start = (" << temp.center.x << "," << temp.center.y << ")" << std::endl;
		console() << "mouse = (" <<mouse.x << "," << mouse.y << ")" << std::endl;
		temp.direction = temp.center - mouse;
		console() << "direc1 = (" << temp.direction.x << "," << temp.direction.y << ")" << std::endl;
		//if(temp.direction != zero)
		temp.direction.normalize();
		console() << "direc2 = (" << temp.direction.x << "," << temp.direction.y << ")" << std::endl;
		dakka.push_back(temp);
	}
	int i = 0;
	for(; i < dakka.size(); ++i) {
		dakka[i].center.x -= dakka[i].direction.x * dakka[i].velocity.x;
		dakka[i].center.y -= dakka[i].direction.y * dakka[i].velocity.y;
	}
	//hero jumping
	if( hero.jumping == false && hero.jumpKey == true) {
		hero.velocity.y = 12;
		hero.jumping = true;
	}
	if( hero.jumping == true ) {
		if ( (hero.center.y - hero.velocity.y) <= (WIND_H - hero.radius) ) {
			hero.center.y -= hero.velocity.y;
			hero.velocity.y -= 1;
		} else {
			hero.jumping = false;
		}
	}
	//left or right
	if(hero.moving == false && hero.leftKey == true) {
		hero.moving = true;
		hero.velocity.x = -10;
	} else if(hero.moving == false && hero.rightKey == true) {
		hero.moving = true;
		hero.velocity.x = 10;
	} else {
		hero.moving = false;
	}
	if(hero.moving == true) {
		hero.center.x = hero.center.x + hero.velocity.x;
	}
}

void SamichIslandApp::draw() {
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	glColor3f(hero.color.r, hero.color.g, hero.color.b);
	gl::drawSolidCircle(hero.center.toV(), hero.radius, 0 ); //hero
	
	//draw bullets
	int i = 0;
	for(; i < dakka.size(); ++i){
		glColor3f(1, 0, 0);
		gl::drawSolidCircle(dakka[i].center.toV(), dakka[i].radius, 0 );
	}
}
CINDER_APP_BASIC( SamichIslandApp, RendererGl )