#include "Headers.h"
#include "SamichIslandApp.h"
#include "DrawEngine.h"

using namespace ci;
using namespace ci::app;
using namespace std;

float game_time, shoot_time, dash_time, punch_time, dash_accel, dash_limit, punch_delay;
int dash_mana_cost;
Vector2 mouse;

void SamichIslandApp::prepareSettings( Settings *settings ){
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 60.0f );
	settings->setWindowPos(10,10);
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
    DrawEngine::get().setBackgroundPath("ikabg.bmp");
    DrawEngine::get().setFrameRate(getFrameRate());
    //initialize states
	appState.setNextState( INIT );
	//time initializations
	game_time = shoot_time = dash_time = punch_time = 0;

	//window bluh
	WIND_H = this->getWindowHeight();
	WIND_W = this->getWindowWidth();

	//hero initialization
	hero.radius = 16;
    hero.health = 100;
    hero.mana = 100;
    hero.maximum_mana = 100;
	hero.center = Vector2(WIND_W/2, WIND_H - hero.radius);
	hero.color = Colorf(1.0,1.0,1.0);
	hero.velocity.x = 12.0;
	hero.velocity.y = 12.0;
	hero.moving = hero.jumping = hero.dashing = hero.punching 
	= hero.jumpKey = hero.leftKey = hero.rightKey = hero.dashKey
	= hero.onPlatform = hero.needsGravity = false;
	JUMP_H = 16;
	hero.damage = 10;
	
	//punch initialization
	punch.radius = 10;
	punch.center = hero.center;
	punch.center.x += hero.radius;
	punch.isRight = true;
	
	punch_delay = 100;
	
	//bullet initialization
	B_RAD = 7.00; B_VEL = 12.00;
	
	//other bullet bluh
	leftClick = false;
	bullet_counter = 50;
	shoot_delay = 500;
	
	//dash thingy
	dash_limit = 800;
    dash_mana_cost = 10;
    
	
	//mooks
	Mook mook;
	mook.health = 100;
	mook.attack = mook.defend = false;
	mook.radius = 25;
	mook.center = Vector2(WIND_H/4,WIND_H-mook.radius);
	mook.color = Colorf(0,1.0,0);
	cannon_fodder.push_back(mook);
	mook.center = Vector2(3*WIND_H/4,WIND_H-mook.radius);
	cannon_fodder.push_back(mook);
	mook.center = Vector2(3*WIND_H/4,WIND_H-mook.radius);
	cannon_fodder.push_back(mook);

	//platform things
	platformA.center = Vector2( WIND_W/2 , 500 );
	platformA.height = 40;
	platformA.width = 300;
	//platformB.center = Vector2( WIND_W*8/10 , 90 );
	//platformB.height = 50;
	//platformB.width = 100;
	platformC.center = Vector2( WIND_W/2 , 240 );
	platformC.height = 40;
	platformC.width = 300;
	platformD.center = Vector2( WIND_W*2/20 , 360 );
	platformD.height = 40;
	platformD.width = 300;
	platformE.center = Vector2( WIND_W*18/20 , 360 );
	platformE.height = 40;
	platformE.width = 300;
	//platformF.center = Vector2( WIND_W*2/10 , 360 );
	//platformF.height = 50;
	//platformF.width = 100;
	//platformG.center = Vector2( WIND_W*8/10 , 430 );
	//platformG.height = 50;
	//platformG.width = 100;
	//platformH.center = Vector2( WIND_W*2/5 , 520 );
	//platformH.height = 50;
	//platformH.width = 100;

    //tower targets
    tower1.center = Vector2( 20, 50 );
    tower1.height = 70;
    tower1.width = 30;
    tower2.center = Vector2( WIND_W - 20 , 50 );
    tower2.height = 70;
    tower2.width = 30;
    
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
		case KeyEvent::KEY_SPACE:
			if (!hero.punching){
				hero.punching = true;
				punch_time = game_time;
			}
		break;
		case KeyEvent::KEY_a:
			hero.leftKey = true;
			if (punch.isRight){
				punch.center.x -= (punch.center.x-hero.center.x)*2;
			}
			punch.isRight = false;
		break;
		case KeyEvent::KEY_d:
			hero.rightKey = true;
			if (!punch.isRight){
				punch.center.x += (hero.center.x-punch.center.x)*2;
			}
			punch.isRight = true;
		break;
		case KeyEvent::KEY_w:
			hero.jumpKey = true;
		break;
		//full screening - WILL BE REMOVED BECAUSE WINDOW SIZE WILL BE CONSTANT
		case KeyEvent::KEY_F12:
        case KeyEvent::KEY_f:
			setFullScreen( ! isFullScreen() );
		break;
		//sizes
		case KeyEvent::KEY_F11:
			if ( isFullScreen() ) 
				setFullScreen( ! isFullScreen() );
			setWindowSize(800,600);
			setWindowPos(100,100);
		break;
		case KeyEvent::KEY_r:
			setup();
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

<<<<<<< HEAD
void SamichIslandApp::update() {	
	//punch
	if (hero.punching){
		int dir = -1;
		if (punch.isRight){
			dir = 1;
		}
		if (punch_time == 0) {
			punch_time = game_time;
		}
		float secs = (game_time-punch_time)/1000;
		if (secs >= 0.118){
			dir *= -1;
		}
		if (secs >= 0.230){
			hero.punching = false;
			punch.center = hero.center;
			punch.center.x += -1*dir*hero.radius;
			punch_time = 0;
		}
		else {
			punch.center.x += dir*((((11*secs)-1.3)*((11*secs)-1.3))+1.5);
		}
	}
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
=======
void SamichIslandApp::update() {
>>>>>>> 313d0acff46247bc0996ddc4a6acd048b976689c

    
    WIND_H = this->getWindowHeight(); WIND_W = this->getWindowWidth();
    //draw engine updates
    DrawEngine::get().setWindowBounds(getWindowBounds());
    
	//state management updates
	bool change = appState.commitState();
    if (change) count = 0;
    
    switch( appState.getCurrentState() ) {
        case INIT:
            if( change ) {
                count = 0; //time between states
                timeout = 50;
            }
            if( count >= timeout ) appState.setNextState( PLAY );
        break;
        case PLAY: {
            //trying gameover state
            if (count % 300 == 0)
            {
                hero.recieveDamage(5);
            }
            
            if (hero.isAlive() == false)
            {
                appState.setNextState(GAMEOVER);
                break;
            }
            
            //punch
			if (hero.punching) {
				int dir = -1;
				if (punch.isRight) dir = 1;
				if (punch_time == 0) punch_time = game_time;

				float secs = (game_time-punch_time)/1000;
				if (secs >= 0.156) dir *= -1;
				if (secs >= 0.313) {
					hero.punching = false;
					punch.center = hero.center;
					punch.center.x += -1*dir*hero.radius;
					punch_time = 0;
				} else punch.center.x += dir*((((9*secs)-1.4)*((9*secs)-1.4))+2);
			}
            
			//dakka dakka dakka!
			Rectf bounds = this->getWindowBounds();
			game_time = ci::app::getElapsedSeconds() * 1000;
			if(leftClick == true && game_time - shoot_time > shoot_delay){
				shoot_time = game_time;
				Bullet temp;
				temp.center = hero.center;
				temp.radius = B_RAD;
				temp.velocity.x = temp.velocity.y = B_VEL;

				temp.direction = temp.center - mouse;
				temp.direction.normalize();
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
			if( (hero.jumping == false && hero.jumpKey == true) ) {
				hero.velocity.y = JUMP_H;
				hero.jumping = true;
			} 

			//jump from the ground
			if( hero.jumping == true && hero.onPlatform == false) {
				if ( (hero.center.y - hero.velocity.y) <= (WIND_H - hero.radius) ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else {
					hero.jumping = false;
				}
			}

			//jump from platforms
			if( hero.jumping == true && hero.onPlatform == true) {
				/*if ( (hero.center.y - hero.velocity.y) <= platformH.center.y - platformH.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;

				if ( (hero.center.y - hero.velocity.y) <= platformG.center.y - platformG.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;

				if ( (hero.center.y - hero.velocity.y) <= platformF.center.y - platformF.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;
*/
				if ( (hero.center.y - hero.velocity.y) <= platformE.center.y - platformE.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;

				if ( (hero.center.y - hero.velocity.y) <= platformD.center.y - platformD.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;

				if ( (hero.center.y - hero.velocity.y) <= platformC.center.y - platformC.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;

				/*if ( (hero.center.y - hero.velocity.y) <= platformB.center.y - platformB.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;*/

				if ( (hero.center.y - hero.velocity.y) <= platformA.center.y - platformA.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;
			}

			//hacky code for missing gravities
			if( hero.onPlatform == false && hero.needsGravity == true ) {
				if ( (hero.center.y - hero.velocity.y) <= (WIND_H - hero.radius) ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else {
					hero.needsGravity = false;
				}
			}

			float normalSpeed = 10; float dashSpeed = 50; float direction;

			//left or right with dashing!!
			if(hero.moving == false && hero.leftKey == true) {
				//check if dash key has been pressed
				if (hero.dashing == false && hero.dashKey == true) {	
					if (hero.mana >= dash_mana_cost) {
                        hero.activateMana(dash_mana_cost);
						hero.dashing = true;
                    }
				} else hero.dashing = false;
				
				hero.moving = true;
				direction = -1;
			} else if( hero.moving == false && hero.rightKey == true) {
				//check if dash key has been pressed
				if (hero.dashing == false && hero.dashKey == true) {
					if (hero.mana >= dash_mana_cost) {
                        hero.activateMana(dash_mana_cost);
						hero.dashing = true;
                    }
				} else hero.dashing = false;

				hero.moving = true;
				direction = 1;
			} else {
				hero.moving = false;
			}
			//if dash key has been pressed, do a dash
			if (hero.dashing == true ){
				dash_accel = 1;
			} else dash_accel = 0;
			//console() << "ifDashing: " << hero.dashing << endl << "DashPressed: " << hero.dashKey << endl;
            
            //regenerate mana
            if (count % 180 == 0 )
            {
                hero.regenerateMana(1);
            }
            
			if(hero.moving == true) {
				hero.velocity.x = (dash_accel * dashSpeed + ( 1 - dash_accel ) * normalSpeed)*direction;
				hero.center.x = hero.center.x + hero.velocity.x;
				punch.center.x += hero.velocity.x;
				//if it goes to leftmost/rightmost of platform, it should fall down
				if(hero.onPlatform == true){
					if ( hero.center.x <= platformA.center.x - platformA.half_width() ||
						hero.center.x >= platformA.center.x + platformA.half_width() ) {
							hero.onPlatform = false;
							hero.needsGravity = true;
					}
					/*if ( hero.center.x <= platformB.center.x - platformB.half_width() ||
						hero.center.x >= platformB.center.x + platformB.half_width() ) {
							hero.onPlatform = false;
							hero.needsGravity = true;
					}*/
					if ( hero.center.x <= platformC.center.x - platformC.half_width() ||
						hero.center.x >= platformC.center.x + platformC.half_width() ) {
							hero.onPlatform = false;
							hero.needsGravity = true;
					}
					if ( hero.center.x <= platformD.center.x - platformD.half_width() ||
						hero.center.x >= platformD.center.x + platformD.half_width() ) {
							hero.onPlatform = false;
							hero.needsGravity = true;
					}
					if ( hero.center.x <= platformE.center.x - platformE.half_width() ||
						hero.center.x >= platformE.center.x + platformE.half_width() ) {
							hero.onPlatform = false;
							hero.needsGravity = true;
					}
				/*	if ( hero.center.x <= platformF.center.x - platformF.half_width() ||
						hero.center.x >= platformF.center.x + platformF.half_width() ) {
							hero.onPlatform = false;
							hero.needsGravity = true;
					}
					if ( hero.center.x <= platformG.center.x - platformG.half_width() ||
						hero.center.x >= platformG.center.x + platformG.half_width() ) {
							hero.onPlatform = false;
							hero.needsGravity = true;
					}
					if ( hero.center.x <= platformH.center.x - platformH.half_width() ||
						hero.center.x >= platformH.center.x + platformH.half_width() ) {
							hero.onPlatform = false;
							hero.needsGravity = true;
					}*/
				}
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
			for (i = 0; i < cannon_fodder.size(); ++i) {
				//if mook and the hero collide for a while
				//if (circleOnCircleDetection(hero, cannon_fodder[i]) && game_time - shoot_time > shoot_delay) {
				//	shoot_time = game_time;
				//	cannon_fodder[i].recieveDamage(hero.damage);
				//}
				if (circleOnCircleDetection(punch, cannon_fodder[i])) cannon_fodder[i].recieveDamage(hero.damage);
				if (cannon_fodder[i].health < 0){
					Drop d;
					d.center = cannon_fodder[i].center;
					//check for different levels in the game.
					d.radius = 5;
					d.floor = Vector2(cannon_fodder[i].center.x,WIND_H-d.radius);
					d.velocity = Vector2(0,1);
					d.color = Colorf(1,1,0);
					drops.push_back(d);
					cannon_fodder.erase(cannon_fodder.begin() + i );
					//cannon_fodder.erase( cannon_fodder.begin() + i );
				}
			}
				
			//platform collisions
			//if(satCircleAABB(hero,platformH)){
			//	//the hero, whether coming from the bottom or the top, will land on the platform
			//	if(hero.center.y + hero.velocity.y <= platformH.center.y - platformH.half_height() ){
			//		hero.center.y = platformH.center.y - platformH.half_height() - hero.radius;
			//		hero.onPlatform = true;
			//	}
			//}
			//if(satCircleAABB(hero,platformG)){
			//	//the hero, whether coming from the bottom or the top, will land on the platform
			//	if(hero.center.y + hero.velocity.y <= platformG.center.y - platformG.half_height() ){
			//		hero.center.y = platformG.center.y - platformG.half_height() - hero.radius;
			//		hero.onPlatform = true;
			//	}
			//}
			//if(satCircleAABB(hero,platformF)){
			//	//the hero, whether coming from the bottom or the top, will land on the platform
			//	if(hero.center.y + hero.velocity.y <= platformF.center.y - platformF.half_height() ){
			//		hero.center.y = platformF.center.y - platformF.half_height() - hero.radius;
			//		hero.onPlatform = true;
			//	}
			//}	
			if(satCircleAABB(hero,platformE)){
				//the hero, whether coming from the bottom or the top, will land on the platform
				if(hero.center.y + hero.velocity.y <= platformE.center.y - platformE.half_height() ){
					hero.center.y = platformE.center.y - platformE.half_height() - hero.radius;
					hero.onPlatform = true;
				}
			}
			if(satCircleAABB(hero,platformD)){
				//the hero, whether coming from the bottom or the top, will land on the platform
				if(hero.center.y + hero.velocity.y <= platformD.center.y - platformD.half_height() ){
					hero.center.y = platformD.center.y - platformD.half_height() - hero.radius;
					hero.onPlatform = true;
				}
			}
			if(satCircleAABB(hero,platformC)){
				//the hero, whether coming from the bottom or the top, will land on the platform
				if(hero.center.y + hero.velocity.y <= platformC.center.y - platformC.half_height() ){
					hero.center.y = platformC.center.y - platformC.half_height() - hero.radius;
					hero.onPlatform = true;
				}
			}
			//if(satCircleAABB(hero,platformB)){
			//	//the hero, whether coming from the bottom or the top, will land on the platform
			//	if(hero.center.y + hero.velocity.y <= platformB.center.y - platformB.half_height() ){
			//		hero.center.y = platformB.center.y - platformB.half_height() - hero.radius;
			//		hero.onPlatform = true;
			//	}
			//}
			if(satCircleAABB(hero,platformA)){
				//the hero, whether coming from the bottom or the top, will land on the platform
				if(hero.center.y + hero.velocity.y <= platformA.center.y - platformA.half_height() ){
					hero.center.y = platformA.center.y - platformA.half_height() - hero.radius;
					hero.onPlatform = true;
				}
			}	

			//tube collisions
        }
        break;
        case GAMEOVER:
        {
            //indeterminate as of now
        }
            break; 
    }
    count++; //increment the time between states
}

void SamichIslandApp::draw() {
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::pushMatrices();
	gl::setMatricesWindow (WIND_W, WIND_H);
	gl::setViewport(this->getWindowBounds());
	gl::enableAlphaBlending();
    
    switch( appState.getCurrentState() ) 
    {
		case INIT: {
			string c = boost::lexical_cast<std::string>(count);
			string t = boost::lexical_cast<std::string>(timeout);    
			gl::drawString( c + " of " + t + " frames", Vec2i( 10, 10 ), Color( 1, 1, 1 ), Font( "Helvetica", 16 ) ); 
		break; }
        case PLAY: {
			int i = 0;
            
            DrawEngine::get().drawSprites();
			//draw punch
			if (hero.punching){
				glColor3f(0, 0, 1);
				gl::drawSolidCircle(punch.center.toV(), punch.radius, 0 );
			}
			
			glColor3f(hero.color.r, hero.color.g, hero.color.b);
			gl::drawSolidCircle(hero.center.toV(), hero.radius, 0 ); //hero
				
			//draw mooks
			for (i = 0; i < cannon_fodder.size(); ++i )
			{
				glColor3f(cannon_fodder[i].color.r,cannon_fodder[i].color.g,cannon_fodder[i].color.b);
				gl::drawSolidCircle(cannon_fodder[i].center.toV(),cannon_fodder[i].radius, 0);
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
			//gl::drawSolidRect(createRectangle(platformB));
			gl::drawSolidRect(createRectangle(platformC));
			gl::drawSolidRect(createRectangle(platformD));
			gl::drawSolidRect(createRectangle(platformE));
			//gl::drawSolidRect(createRectangle(platformF));
			//gl::drawSolidRect(createRectangle(platformG));
			//gl::drawSolidRect(createRectangle(platformH));

			//draw bullets
			for(; i < dakka.size(); ++i) {
				glColor3f(1, 0, 0);
				gl::drawSolidCircle(dakka[i].center.toV(), dakka[i].radius, 0 );
			}

			//tubes
			glColor3f(1,1,0);
			gl::drawSolidRect(createRectangle(tubeA));
			gl::drawSolidRect(createRectangle(tubeB));
            
            //tower
            glColor3f(1,0,1);
            gl::drawSolidRect(createRectangle(tower1));
            gl::drawSolidRect(createRectangle(tower2));
            
            string hero_mana = boost::lexical_cast<std::string>(hero.mana);
            string hero_maxmana = boost::lexical_cast<std::string>(hero.maximum_mana);
            string hero_health = boost::lexical_cast<std::string>(hero.health);
            DrawEngine::get().drawString( hero_mana + " of " + hero_maxmana + " mana.", Vector2( 10, 20 ), Color ( 0, 0, 0 ), Font("Helvetica", 16), TEXT_LEFT );
            DrawEngine::get().drawString( hero_health + " of 100", Vector2( 10, 40 ), Color ( 0, 0, 0 ), Font("Helvetica", 16), TEXT_LEFT );
        }
        break;
        case GAMEOVER: {
            gl::drawStringCentered( "GAME OVER!", getWindowCenter(), Color (1,1,1), Font ("Helvetica", 24));
            
        }
        break;
        
	}
	gl::popMatrices();
}
CINDER_APP_BASIC( SamichIslandApp, RendererGl )