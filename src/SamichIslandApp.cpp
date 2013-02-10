#include "Headers.h"
#include "SamichIslandApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const float PI = 3.141592653589793238462643383279502884197;

void SamichIslandApp::prepareSettings( Settings *settings ){
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 60.0f );
}

void SamichIslandApp::setup()
{	
	WIND_H = this->getWindowHeight();
	WIND_W = this->getWindowWidth();
	moving = jumping = jumpKey = leftKey = rightKey = leftClick = false;
	bulletSpeed = 20;
	player.radius = 20;
	player.center = Vector2(WIND_W/2, WIND_H - player.radius);
	player.color = Colorf(1.0,1.0,1.0);
	player.velocity.x = 12.0;
	player.velocity.y = 12.0;
	int i =  0;
	for(; i < 8; ++i){
		bullets[i].center = Vector2(0,0);
		bullets[i].radius = 10;
	}
}

void SamichIslandApp::keyDown( KeyEvent event )
{
	int code = event.getCode();
	if (code==KeyEvent::KEY_a) {
		leftKey = true;
	}

	if (code==KeyEvent::KEY_d) {
		rightKey = true;
	}

	if (code==KeyEvent::KEY_SPACE) {
		jumpKey = true;
	}
}

void SamichIslandApp::keyUp( KeyEvent event ){
	int code = event.getCode();
	if (code==KeyEvent::KEY_a) {
		leftKey = false;
	}

	if (code==KeyEvent::KEY_d) {
		rightKey = false;
	}

	if (code==KeyEvent::KEY_SPACE) {
		jumpKey = false;
	}
}

void SamichIslandApp::mouseDown( MouseEvent event )
{
	leftClick = true;
	scale.x = event.getX();
	scale.y = event.getY();
}

void SamichIslandApp::mouseUp( MouseEvent event )
{
	leftClick = false;
}

void SamichIslandApp::update()
{	
	//firing bullets
	if(leftClick == true){
		int i = 0;
		float dp = scale*player.center; //dot product
		angle = ( 180 * (acos ( dp / ( scale.mag() * player.center.mag() ) ) ) ) / PI;
		float xVel = cos(angle) * bulletSpeed;
		float yVel = sin(angle) * bulletSpeed;
		for(; i < 8; ++i){
			bullets[i].center.x = xVel;
			bullets[i].center.y = yVel;
			console() << "angle: " << angle << std::endl;
			console() << "velocity: (" << xVel << "," << yVel << ")" << std::endl;
			console() << "position: (" << bullets[i].center.x << "," << bullets[i].center.y << ")" << std::endl;
		}
	}
	//player jumping
	if( jumping == false && jumpKey == true) {
		player.velocity.y = 12;
		jumping = true;
	}
	if( jumping == true ) {
		if ( (player.center.y - player.velocity.y) <= (WIND_H - player.radius) ) {
			player.center.y -= player.velocity.y;
			player.velocity.y -= 1;
		} else {
			jumping = false;
		}
	}
	//left or right
	if(moving == false && leftKey == true) {
		moving = true;
		player.velocity.x = -10;
	} else if(moving == false && rightKey == true) {
		moving = true;
		player.velocity.x = 10;
	} else {
		moving = false;
	}
	if(moving == true) {
		player.center.x = player.center.x + player.velocity.x;
	}
}

void SamichIslandApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	glColor3f(player.color.r, player.color.g, player.color.b);
	gl::drawSolidCircle(player.center.toV(), player.radius, 0 ); //hero
	
	int i = 0;
	for(; i < 8; ++i){
		gl::drawSolidCircle(bullets[i].center.toV(), bullets[i].radius, 0 ); //bullets
	}
}

CINDER_APP_BASIC( SamichIslandApp, RendererGl )
