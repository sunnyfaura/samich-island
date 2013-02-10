#include "Headers.h"
#include "SamichIslandApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

float WIND_H, WIND_W; //window details

bool moving = false, jumping = false; //any changes in player state
bool jumpKey = false, leftKey = false, rightKey = false; //movement details

void SamichIslandApp::prepareSettings( Settings *settings ){
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 60.0f );
}

void SamichIslandApp::setup()
{	
	WIND_H = this->getWindowHeight();
	WIND_W = this->getWindowWidth();
	player.center = Vector2(10, WIND_H - 10);
	player.color = Colorf(1.0,1.0,1.0);
	player.radius = 10;
	player.velocity = Vector2(6.0,6.0);
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

void SamichIslandApp::mouseMove( MouseEvent event )
{
	
}

void SamichIslandApp::update()
{	
	//player jumping
	console() << "(" << player.center.x << "," << player.center.y << ")" << std::endl;
	if( jumping == false && jumpKey == true) {
		player.velocity.y = 20;
		jumping = true;
	}
	if( jumping == true ) {
		if ( player.center.y - player.velocity.y < WIND_H ) {
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
	gl::drawSolidCircle(player.center.toV(), player.radius, 0 );
}

CINDER_APP_BASIC( SamichIslandApp, RendererGl )
