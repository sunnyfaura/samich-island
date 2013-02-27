#include "Headers.h"
#include "SamichIslandApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

float game_time, shoot_time, dash_time;
Vector2 mouse;

void SamichIslandApp::prepareSettings( Settings *settings ){
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 60.0f );
	settings->setWindowPos(100,100);
	settings->setResizable( false );
}

void SamichIslandApp::resize(ResizeEvent event)
{
	WIND_W = event.getWidth();
	WIND_H = event.getHeight();
	//console() << "WIDTH:" << WIND_W << endl;
	//console() << "HEIGHT:" << WIND_H << endl;
	gl::setMatricesWindow (WIND_W, WIND_H);
	mCam.setPerspective( 45, getWindowAspectRatio(), 1, 100 );
	gl::setMatrices( mCam );
}

void SamichIslandApp::setup()
{	
	game_time = shoot_time = dash_time = 0;
	//window bluh
	WIND_H = this->getWindowHeight();
	WIND_W = this->getWindowWidth();
	//hero initialization
	hero.radius = 16;
	hero.center = Vector2(WIND_W/2, WIND_H - hero.radius);
	hero.color = Colorf(1.0,1.0,1.0);
	hero.velocity.x = 12.0;
	hero.velocity.y = 12.0;
	hero.moving = hero.jumping = hero.dashing = hero.jumpKey = hero.leftKey = hero.rightKey = hero.dashKey = false;
	JUMP_H = 16;
	hero.damage = 10;
	hero.dashDir = 0;
	//bullet initialization
	B_RAD = 7.00; B_VEL = 12.00;
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
	mook.center = Vector2(3*WIND_H/4,WIND_H-mook.radius);
	mooker.push_back(mook);

	//platform things
	platformA.center = Vector2( WIND_W*2/10 , 90 );
	platformA.height = 20;
	platformA.width = 310;
	platformB.center = Vector2( WIND_W*8/10 , 90 );
	platformB.height = 20;
	platformB.width = 310;
	platformC.center = Vector2( WIND_W/2 , 180 );
	platformC.height = 20;
	platformC.width = 200;
	platformD.center = Vector2( WIND_W*2/20 , 270 );
	platformD.height = 20;
	platformD.width = 150;
	platformE.center = Vector2( WIND_W*18/20 , 270 );
	platformE.height = 20;
	platformE.width = 150;
	platformF.center = Vector2( WIND_W*2/10 , 360 );
	platformF.height = 20;
	platformF.width = 310;
	platformG.center = Vector2( WIND_W*8/10 , 430 );
	platformG.height = 20;
	platformG.width = 310;
	platformH.center = Vector2( WIND_W*2/5 , 520 );
	platformH.height = 20;
	platformH.width = 200;

	//tubes
	tubeA.height = 50;
	tubeA.width = 50;
	tubeA.center = Vector2( tubeA.width/2 , WIND_H-(tubeA.height/2) );
	tubeB.height = 50;
	tubeB.width = 50;
	tubeB.center = Vector2( WIND_W-(tubeB.width/2) , WIND_H-(tubeB.height/2) );
}

void SamichIslandApp::keyDown( KeyEvent event ) {
	int code = event.getCode();
	if (event.isShiftDown() == true){
		hero.dashKey = true;
	}
	switch (code) {
		case KeyEvent::KEY_a:
			hero.leftKey = true;
		break;
		case KeyEvent::KEY_d:
			hero.rightKey = true;
		break;
		case KeyEvent::KEY_w:
			hero.jumpKey = true;
		break;
		//full screening
		case KeyEvent::KEY_F12:
			setFullScreen( ! isFullScreen() );
		break;
		//sizes
		case KeyEvent::KEY_F11:
			if ( isFullScreen() )
				setFullScreen( ! isFullScreen() );
			setWindowSize(800,600);
			setWindowPos(100,100);
		break;
		case KeyEvent::KEY_F10:
			if ( isFullScreen() ) 
				setFullScreen( ! isFullScreen() );
			setWindowSize(640,480);
			setWindowPos(100,100);
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
	if (event.isShiftDown() == false){
		hero.dashKey = false;
	}
	switch (code) {
		case KeyEvent::KEY_a:
			hero.leftKey = false;
		break;
		case KeyEvent::KEY_d:
			hero.rightKey = false;
		break;
		case KeyEvent::KEY_w:
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
	//punch
	
	//dakka dakka dakka!
	Rectf bounds = this->getWindowBounds();
	game_time = ci::app::getElapsedSeconds() * 1000;
	if(leftClick == true && game_time - shoot_time > shoot_delay){
		shoot_time = game_time;
		Bullet temp;
		temp.center = hero.center;
		temp.radius = B_RAD;
		temp.velocity.x = temp.velocity.y = B_VEL;

		//console() << "start = (" << temp.center.x << "," << temp.center.y << ")" << std::endl;
		//console() << "mouse = (" <<mouse.x << "," << mouse.y << ")" << std::endl;
		temp.direction = temp.center - mouse;
		//console() << "direc1 = (" << temp.direction.x << "," << temp.direction.y << ")" << std::endl;
		//if(temp.direction != zero)
		temp.direction.normalize();
		//console() << "direc2 = (" << temp.direction.x << "," << temp.direction.y << ")" << std::endl;
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
		hero.velocity.y = JUMP_H;
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
	//dashing
	if (hero.dashKey == true && hero.dashing == false && (hero.leftKey == true || hero.rightKey == true)){//start dash
		console() << "dash" << endl;
		hero.dashing = true;
		hero.dashDir = 10;
		if (hero.rightKey && !hero.leftKey){//getting dash direction
			hero.dashDir = -10;
		}
	}
	if (hero.dashing == true){//continuous dash uninteruptted
		if (dash_time == 0){
			dash_time = game_time;
		}
		hero.moving = true;
		float t = (game_time-dash_time)/1000;
		hero.velocity.x = (-pow(((6*t)-2.5), 2)+5)*hero.dashDir;
		console() << hero.velocity.x << endl;
		if (hero.velocity.x < 10){ //end dash
			hero.dashing = false;
			hero.moving = false;
			dash_time = 0;
			hero.dashDir = 0;
		}
	}
	//left or right
	else if(hero.moving == false && hero.leftKey == true) {
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
				//drops.erase ( drops.begin() + i );
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
	
	//platform collisions

	//tube collisions
	
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

	//platforms
	glColor3f(0,1,1);
	gl::drawSolidRect(createRectangle(platformA));
	gl::drawSolidRect(createRectangle(platformB));
	gl::drawSolidRect(createRectangle(platformC));
	gl::drawSolidRect(createRectangle(platformD));
	gl::drawSolidRect(createRectangle(platformE));
	gl::drawSolidRect(createRectangle(platformF));
	gl::drawSolidRect(createRectangle(platformG));
	gl::drawSolidRect(createRectangle(platformH));
	//tubes
	glColor3f(1,1,0);
	gl::drawSolidRect(createRectangle(tubeA));
	gl::drawSolidRect(createRectangle(tubeB));
	gl::popMatrices();
}
CINDER_APP_BASIC( SamichIslandApp, RendererGl )