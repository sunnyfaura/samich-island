#include "Headers.h"
#include "SamichIslandApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void SamichIslandApp::prepareSettings( Settings *settings ){
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 60.0f );
}

void SamichIslandApp::setup()
{
	player.center = Vector2(this->getWindowWidth(), 0 );
	player.color = Colorf(1.0,1.0,1.0);
	player.radius = 10;
	player.velocity = Vector2(40.0,40.0);
	
	Circle e;
	e.radius = 20;
	for (size_t i = 0; i < NUM_ENEMIES ; ++i) {
		//e.center = Vector2(randFloat()*this->getWindowWidth(),randFloat()*this->getWindowHeight()/2);
		e.center = Vector2(80,400);
		
		e.color = Color(randFloat(), randFloat(), 0.0);
		enemies.push_back(e);
	}
}

void playerMovement( Circle *player, int code )
{
	if(code==KeyEvent::KEY_w)
	{
		(*player).center.y -= (*player).velocity.y;
	}
	
	if (code==KeyEvent::KEY_a)
	{
		(*player).center.x -= (*player).velocity.x;
	}
	
	if (code==KeyEvent::KEY_s)
	{
		(*player).center.y += (*player).velocity.y;
	}
	
	if (code==KeyEvent::KEY_d)
	{
		(*player).center.x += (*player).velocity.x;
	}
}

void SamichIslandApp::keyDown( KeyEvent event )
{
	playerMovement(&player, event.getCode());
}

void SamichIslandApp::mouseMove( MouseEvent event )
{
	player.center = Vector2(event.getX(), event.getY());
}

void SamichIslandApp::update()
{
	for (size_t i = 0; i < NUM_ENEMIES; ++i)
	{
		Vector2 player_distance = player.center - enemies[i].center;
		player_distance.normalize();
		
		enemies[i].center += player_distance * 4;
	}
}

void SamichIslandApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	glColor3f(player.color.r, player.color.g, player.color.b);
	gl::drawSolidCircle(player.center.toV(), player.radius, 0 );
	
	for (size_t i = 0; i < enemies.size(); ++i) {
		glColor3f(enemies[i].color.r, enemies[i].color.g, enemies[i].color.b);
		gl::drawSolidCircle(enemies[i].center.toV(), enemies[i].radius, 0 );
	}
}

CINDER_APP_BASIC( SamichIslandApp, RendererGl )
