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
	settings->setWindowPos(0,0);
	settings->setResizable( false );
}

void SamichIslandApp::resize(ResizeEvent event)
{
	/*WIND_W = event.getWidth();
	WIND_H = event.getHeight();
	console() << "WIDTH:" << WIND_W << endl;
	console() << "HEIGHT:" << WIND_H << endl;
	gl::setMatricesWindow (WIND_W, WIND_H);*/

	mCam.setPerspective( 45, getWindowAspectRatio(), 1, 100 );
	gl::setMatrices( mCam );
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

	//mook initialization
	Mook mook;
	mook.health = 100;
	mook.attack = mook.defend = false;
	mook.radius = 25;
	mook.center = Vector2(WIND_H/4,WIND_H-mook.radius);
	mook.color = Colorf(0,1.0,0);
	mooker.push_back(mook);
	
	mook.center = Vector2(3*WIND_H/4,WIND_H-mook.radius);
	mooker.push_back(mook);
}



void SamichIslandApp::keyDown( KeyEvent event ) {
	int code = event.getCode();
	switch (code) {
		case KeyEvent::KEY_a:
			hero.leftKey = true;
		break;
		case KeyEvent::KEY_d:
			hero.rightKey = true;
		break;
		case KeyEvent::KEY_SPACE:
			hero.jumpKey = true;
		break;
		//full screening
		case KeyEvent::KEY_f:
			setFullScreen( ! isFullScreen() );
		break;
		//sizes
		case KeyEvent::KEY_c:
			if ( isFullScreen() ) 
				setFullScreen( ! isFullScreen() );
			setWindowSize(640,480);
			setWindowPos(0,0);
				
		break;
		case KeyEvent::KEY_t:
			if ( isFullScreen() )
				setFullScreen( ! isFullScreen() );
			setWindowSize(800,600);
			setWindowPos(0,0);
		break;
		case KeyEvent::KEY_ESCAPE:
			quit();
		break;
				
		default:
		break;
	}
}

void SamichIslandApp::keyUp( KeyEvent event ) {

	int code = event.getCode();
	switch (code) {
		case KeyEvent::KEY_a:
			hero.leftKey = false;
		break;
		case KeyEvent::KEY_d:
			hero.rightKey = false;
		break;
		case KeyEvent::KEY_SPACE:
			hero.jumpKey = false;
				
		default:
		break;
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
	Rectf bounds = this->getWindowBounds();
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
		
		//remove bullet if outside window
		if (!bounds.contains(dakka[i].center.toV())) {
			if (!dakka.empty())
				dakka.erase(dakka.begin() + i);
		}

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

	//get drops
	for (i = 0; i < drops.size(); ++i) {
		if (circleOnCircleDetection(hero, drops[i])) {
				//drop effects here
				drops.erase ( drops.begin() + i );
		}
		
		//dropping effect to "floor" 
		if ( drops[i].center.y <= drops[i].floor.y ) {
			drops[i].center.y += drops[i].velocity.y;
		}
		else {
			drops[i].center = drops[i].floor; 
		} 
	}

	//mooks
	for (i = 0; i < mooker.size(); ++i) {
		if (circleOnCircleDetection(hero, mooker[i]) && game_time - shoot_time > shoot_delay) 
		{
			shoot_time = game_time;
			mooker[i].recieveDamage(hero.damage);
		}
		
		if (mooker[i].health < 0)
		{
				Drop d;
				d.center = mooker[i].center;
				//check for different levels in the game.
				d.radius = 5;
				d.floor = Vector2(mooker[i].center.x,WIND_H-d.radius);
				d.velocity = Vector2(0,1);
				d.color = Colorf(1,1,0);
				drops.push_back(d);
		
				mooker.erase( mooker.begin() + i );
		}
	}
}

void SamichIslandApp::draw() {
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
		gl::pushMatrices();
		gl::setMatricesWindow (WIND_W, WIND_H);
		gl::setViewport(this->getWindowBounds());
		
		glColor3f(hero.color.r, hero.color.g, hero.color.b);
		gl::drawSolidCircle(hero.center.toV(), hero.radius, 0 ); //hero
		
		//draw bullets
		int i = 0;
		for(; i < dakka.size(); ++i){
			glColor3f(1, 0, 0);
			gl::drawSolidCircle(dakka[i].center.toV(), dakka[i].radius, 0 );
		}
		
		//draw mooks
		for (i = 0; i < mooker.size(); ++i )
		{
			glColor3f(mooker[i].color.r,mooker[i].color.g,mooker[i].color.b);
			gl::drawSolidCircle(mooker[i].center.toV(),mooker[i].radius, 0);
		}
		
		//draw drops
		for (i = 0; i < drops.size(); ++i )
		{
			glColor3f(drops[i].color.r,drops[i].color.g,drops[i].color.b);
			gl::drawSolidCircle(drops[i].center.toV(),drops[i].radius, 0);
		}
	gl::popMatrices();
	
}
CINDER_APP_BASIC( SamichIslandApp, RendererGl )