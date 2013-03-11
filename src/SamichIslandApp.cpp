#include "Headers.h"
#include "SamichIslandApp.h"
#include "DrawEngine.h"

using namespace ci;
using namespace ci::app;
using namespace std;

float game_time, shoot_time;
float units_per_second = 5, mook_elapsed_seconds, creation_rate, spawn_unit_count;
float dash_time, punch_time, dash_accel, dash_limit, punch_delay;

int dash_mana_cost;
Vector2 mouse;

void SamichIslandApp::prepareSettings( Settings *settings ) {
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 60.0f );
	settings->setWindowPos(10,10);
	settings->setResizable( false );
}

void SamichIslandApp::resize(ResizeEvent event) {
	mCam.setPerspective( 45, getWindowAspectRatio(), 1, 100 );
	gl::setMatrices( mCam );
}

void SamichIslandApp::setup(){
    //menu setup
    menuGUI = new ciUICanvas(0,0,getWindowWidth(),getWindowHeight());
    menuGUI->addWidgetDown(new ciUILabelButton(getWindowWidth()-CI_UI_GLOBAL_WIDGET_SPACING*2, false, "PLAY", CI_UI_FONT_LARGE));
    menuGUI->addWidgetDown(new ciUILabelButton(getWindowWidth()-CI_UI_GLOBAL_WIDGET_SPACING*2, false, "LEADERBOARDS", CI_UI_FONT_LARGE));
    menuGUI->addWidgetDown(new ciUILabelButton(getWindowWidth()-CI_UI_GLOBAL_WIDGET_SPACING*2, false, "EXIT", CI_UI_FONT_LARGE));
    menuGUI->autoSizeToFitWidgets();
    menuGUI->registerUIEvents(this, &SamichIslandApp::guiEvent);
    //drawengine setup
    DrawEngine::get().setBackgroundPath("ikabg.bmp");
    DrawEngine::get().setFrameRate(getFrameRate());
	appState.setNextState( INIT );
	//time initializations
	game_time = shoot_time = dash_time = punch_time = 0;
	//window bluh//
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
	= hero.needsGravity = false;
	hero.on_btm_platform = hero.on_left_platform = hero.on_right_platform = hero.on_top_platform = false;
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
	leftClick = false;
	bullet_counter = 50;
	shoot_delay = 500;
	//dash thingy
	dash_limit = 800;
    dash_mana_cost = 10;
	//portal from hell
    portal.center = Vector2(WIND_W/2,100);
    portal.width = portal.height = 150;
	//mooks
    MAX_MOOKS = 25;
	for(int i = 0; i < MAX_MOOKS; ++i){
		Mook mook;
		mook.health = 100;
		mook.radius = 25;
		mook.center = portal.center;
		cannon_fodder.push_back(mook);
	}
	for(int i = 0; i < cannon_fodder.size(); ++i) { //randomize direction
		float temp = ( -1+2*((float)rand())/RAND_MAX );
		if(temp < 0) cannon_fodder[i].direction = -1;
		else cannon_fodder[i].direction = 1;
		//console() << cannon_fodder[i].direction << std::endl;
		cannon_fodder[i].next = Vector2( this->getWindowWidth()/2 , this->getWindowHeight() );
		cannon_fodder[i].lerp_time = 0;
	}
	spawn_unit_count = MAX_MOOKS;
	//platform things
	bottom_platform.center = Vector2( WIND_W/2 , 500 );
	bottom_platform.height = 40;
	bottom_platform.width = 300;
	top_platform.center = Vector2( WIND_W/2 , 240 );
	top_platform.height = 40;
	top_platform.width = 300;
	left_platform.center = Vector2( WIND_W*2/20 , 360 );
	left_platform.height = 40;
	left_platform.width = 300;
	right_platform.center = Vector2( WIND_W*18/20 , 360 );
	right_platform.height = 40;
	right_platform.width = 300;
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
	switch(appState.getCurrentState()){
		case INIT: 
			appState.setNextState( MENU );
		break;
		case PLAY:
			int code = event.getCode();
			if (event.isShiftDown() == true)
				hero.dashKey = true;
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
			}
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
    if (event.isLeft()) {
        switch(appState.getCurrentState()){
            case INIT: 
                appState.setNextState( MENU );
            break;
            case PLAY:
                leftClick = true;
            break;
            default:
                leftClick = false;
            break;
        }
    }
}

void SamichIslandApp::mouseUp( MouseEvent event ) {
	leftClick = false;
}

void SamichIslandApp::guiEvent(ciUIEvent *event) {
    string name=event->widget->getName();
    switch(appState.getCurrentState())
    {
        case MENU: {
            if (name == "PLAY") {
                appState.setNextState(PLAY);
            }
            else if (name == "EXIT") {
                quit();
            }
        break;}
        default:
        break;
    }
}

void SamichIslandApp::update() {
	//console() << "jumping=" << hero.jumping << " needsgravity=" << hero.needsGravity <<
	//	 "onBtm=" << hero.on_btm_platform << " onLeft=" << hero.on_left_platform << " onRight=" << hero.on_right_platform << " onTop=" << hero.on_top_platform 
	//	 << std::endl;
	//console() << "ifDashing: " << hero.dashing << endl << "DashPressed: " << hero.dashKey << endl;

    WIND_H = this->getWindowHeight(); WIND_W = this->getWindowWidth();

    //draw engine updates
    DrawEngine::get().setWindowBounds(getWindowBounds());
    
	//state management updates
	bool change = appState.commitState();
    if (change) count = 0;
    
    switch( appState.getCurrentState() ) {
        case INIT:
			//bluh
        break;
        case MENU: {
            //menu update
            menuGUI->update();
        }
        case PLAY: {
            //trying gameover state
            if (count % 300 == 0)
                hero.recieveDamage(5);
            if (hero.isAlive() == false) {
                appState.setNextState(GAMEOVER);
                break; }

//**=======================================EVERYTHING RELATED TO HERO============================================**//
            
           	//hero to-do-list/pseudocode
            //limit the hero to game boundaries 
            //fix the bullet shooting: check if the menu is in PLAY (not anywhere else),
            //hero inflict damage by either shoot or punch or dash
            //define the power of punch and shoot and dash
            //colliding with a mook will decrease hero life (do not repeat collisions)

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
			} else punch.center = hero.center;
            
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
                
                //bullet decreases mana
                if (hero.sufficientMana(5)) {
                    hero.activateMana(5); //bullet is fired decreases mana prolly game constant
                    dakka.push_back(temp);
                }
			}

			int i = 0;
			for(; i < dakka.size(); ++i) {
                Bullet &b = dakka[i];
				dakka[i].center.x -= dakka[i].direction.x * dakka[i].velocity.x;
				dakka[i].center.y -= dakka[i].direction.y * dakka[i].velocity.y;	
				//remove bullet if outside window
				if (!bounds.contains(dakka[i].center.toV())) {
					if (!dakka.empty())
						dakka.erase(dakka.begin() + i);
				}
                
                /*for (int j = 0; j < cannon_fodder.size(); ++j) {
                    Mook &m = cannon_fodder[i];
                    if (circleOnCircleDetection( m, b ) )
                    {
                        m.recieveDamage(20);
                    }
                    
                    if(m.isAlive() == false)
                    {
                        cannon_fodder.erase(cannon_fodder.begin() + j);
                    }
                }*/
                
                
			}
				
			//hero jumping
			if( (hero.jumping == false && hero.jumpKey == true) ) {
				hero.velocity.y = JUMP_H;
				hero.jumping = true;
				punch.velocity.y = JUMP_H;
			}

			//jump from the ground
			if( hero.jumping == true 
				&& hero.on_btm_platform == false && hero.on_left_platform == false 
				&& hero.on_right_platform == false && hero.on_top_platform == false) {
				if ( (hero.center.y - hero.velocity.y) <= WIND_H ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
					punch.center.y -= 1;
				} else {
					hero.jumping = false;
				}
			}

			//jump from platforms			
			if( hero.jumping == true && hero.on_btm_platform == true) {
				if( (hero.center.y - hero.velocity.y) <= bottom_platform.center.y - bottom_platform.half_height()) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
					punch.center.y -= 1;
				} else hero.jumping = false;
			}
			if( hero.jumping == true && hero.on_left_platform == true) {
				if ( (hero.center.y - hero.velocity.y) <= left_platform.center.y - left_platform.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
					punch.center.y -= 1;
				} else hero.jumping = false;
			}
			if( hero.jumping == true && hero.on_right_platform == true) {
				if( (hero.center.y - hero.velocity.y) <= right_platform.center.y - right_platform.half_height()) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
					punch.center.y -= 1;
				} else hero.jumping = false;
			}
			if( hero.jumping == true && hero.on_top_platform == true) {
				if ( (hero.center.y - hero.velocity.y) <= top_platform.center.y - top_platform.half_height() ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
					punch.center.y -= 1;
				} else hero.jumping = false;
			}

			//hacky code for missing gravities
			if( hero.on_btm_platform == false && hero.on_left_platform == false && hero.on_right_platform == false && hero.on_top_platform == false
				&& hero.needsGravity == true ) {
				if ( (hero.center.y - hero.velocity.y) <= WIND_H  ) {
					hero.center.y -= hero.velocity.y;
					punch.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
					punch.center.y -= 1;
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
			} else hero.moving = false;

			//if dash key has been pressed, do a dash
			if (hero.dashing == true ) dash_accel = 1;
			else dash_accel = 0; 
            
            //regenerate mana
            if (count % 180 == 0 ) hero.regenerateMana(1);
            
			if(hero.moving == true) {
				hero.velocity.x = (dash_accel * dashSpeed + ( 1 - dash_accel ) * normalSpeed)*direction;
				hero.center.x = hero.center.x + hero.velocity.x;
				punch.center.x += hero.velocity.x;
				//if it goes to leftmost/rightmost of platform, it should fall down
				if(hero.on_btm_platform == true){
					if ( hero.center.x <= bottom_platform.center.x - bottom_platform.half_width() ||
						hero.center.x >= bottom_platform.center.x + bottom_platform.half_width() ) {
							hero.on_btm_platform = false;
							hero.needsGravity = true;
					}
				}
				if(hero.on_left_platform == true){
					if ( hero.center.x <= left_platform.center.x - left_platform.half_width() ||
						hero.center.x >= left_platform.center.x + left_platform.half_width() ) {
							hero.on_left_platform = false;
							hero.needsGravity = true;
					}
				}
				if(hero.on_right_platform == true){
					if ( hero.center.x <= right_platform.center.x - right_platform.half_width() ||
						hero.center.x >= right_platform.center.x + right_platform.half_width() ) {
							hero.on_right_platform = false;
							hero.needsGravity = true;
					}
				}
				if(hero.on_top_platform == true){
					if ( hero.center.x <= top_platform.center.x - top_platform.half_width() ||
						hero.center.x >= top_platform.center.x + top_platform.half_width() ) {
							hero.on_top_platform = false;
							hero.needsGravity = true;
					}
				}
			}
			
//**=======================================EVERYTHING RELATED TO MOOK===========================================**//

			//mook to-do-list/pseudocode

			//have 25 mooks with 5 starting health in a queue
			//dequeue everytime they leave the portal
			//lerp either to the left or to the right
			//make the mook attracted to gravity
			//make the mook be affected by platforms
			//if it has reached an end of a platform, make it reverse direction
			//if it has reached a tube, add 5 to mook's health
			//everytime a mook enters a tube, that specific mook increases max_health
			//increase each color of that mook by 1
			//queue every time they enter a tube or get killed
			//every time they get killed, they return to minimum life
			//colliding mook with a hero will decrease the life of hero
			
			// int j = spawn_unit_count-1;
			
			// while(spawn_unit_count > 0) {
			// 	mook_elapsed_seconds = ci::app::getElapsedSeconds() * 1000;
			// 	creation_rate += units_per_second * mook_elapsed_seconds;
	  //     		while ( creation_rate >= 1 ) {
	  //     			creation_rate--;
	  //     			//make a unit
	  //     			cannon_fodder[j].lerp_time += 1.0f;
			// 		if( cannon_fodder[j].lerp_time > 100 ) {
			// 			cannon_fodder[j].lerp_time = 0;
			// 			cannon_fodder[j].prev.x = (float)rand()/RAND_MAX * this->getWindowWidth();
			// 			cannon_fodder[j].prev.y = (float)rand()/RAND_MAX * this->getWindowHeight();
			// 		}
	  //     			spawn_unit_count--;
	  //     			if(spawn_unit_count == 0) {
	  //     				creation_rate = 0;
	  //     			}
	  //     		}
   //    		}

			for( int j = 0; j < MAX_MOOKS; ++j){
				cannon_fodder[j].lerp_time += 1.0f;
				if( cannon_fodder[j].lerp_time > 100 ) {
					cannon_fodder[j].lerp_time = 0;
					cannon_fodder[j].prev.x = (float)rand()/RAND_MAX * this->getWindowWidth();
					cannon_fodder[j].prev.y = (float)rand()/RAND_MAX * this->getWindowHeight();
				}
			}

			//mook dropping drops
			for (int i = 0; i < cannon_fodder.size(); ++i) {
				if (circleOnCircleDetection(punch, cannon_fodder[i]))
					cannon_fodder[i].recieveDamage(hero.damage);
				if (cannon_fodder[i].health < 0){
					Drop d;
					d.center = cannon_fodder[i].center;
					//check for different levels in the game.
					d.radius = 5;
					d.velocity = Vector2(0,1);
					d.color = Colorf(1,1,0);
					drops.push_back(d);
					cannon_fodder.erase(cannon_fodder.begin() + i );
					//cannon_fodder.erase( cannon_fodder.begin() + i );
				}
			}

			//get drops
			for (int i = 0; i < drops.size(); ++i) {
                Drop &d = drops[i];
				if (circleOnCircleDetection(hero, drops[i])) {
						//drop effects here
						drops.erase ( drops.begin() + i );
				}
					
				//dropping platform collision detection
				if ( d.center.y - d.velocity.y <= WIND_H - d.radius )
                {
                    if (satCircleAABB(d, top_platform))
                    {
                        d.velocity.y = 0;
                        d.center.y = top_platform.center.y - top_platform.half_height() - d.radius;
                    }
                    else if (satCircleAABB(d, bottom_platform))
                    {
                        d.velocity.y = 0;
                        d.center.y = bottom_platform.center.y - bottom_platform.half_height() - d.radius;
                    }
                    else if (satCircleAABB(d, left_platform))
                    {
                        d.velocity.y = 0;
                        d.center.y = left_platform.center.y - left_platform.half_height() - d.radius;
                    }
                    else if (satCircleAABB(d, top_platform))
                    {
                        d.velocity.y = 0;
                        d.center.y = right_platform.center.y - right_platform.half_height() - d.radius;
                    }
                }
                
                d.center += d.velocity;
                
			}
				
//**==================================EVERYTHING NOT RELATED TO HERO OR MOOK===================================**//

			//platform collisions
			if(satCircleAABB(hero,bottom_platform)){
				if(hero.center.y + hero.velocity.y <= bottom_platform.center.y - bottom_platform.half_height() ){
					hero.center.y = bottom_platform.center.y - bottom_platform.half_height() - hero.radius;
					hero.on_btm_platform = true;
				} else if(hero.center.y + hero.velocity.y > bottom_platform.center.y + bottom_platform.half_height() ){
					hero.center.y = bottom_platform.center.y + bottom_platform.half_height() - hero.radius;
					hero.velocity.y *= -1;
					hero.on_btm_platform = false;
				}
			}
			if(satCircleAABB(hero,left_platform)){
				if(hero.center.y + hero.velocity.y <= left_platform.center.y - left_platform.half_height() ){
					hero.center.y = left_platform.center.y - left_platform.half_height() - hero.radius;
					hero.on_left_platform = true;
				} else if(hero.center.y + hero.velocity.y > left_platform.center.y + left_platform.half_height() ){
					hero.center.y = left_platform.center.y + left_platform.half_height() - hero.radius;
					hero.velocity.y *= -1;
					hero.on_left_platform = false;
				}
			}
			if(satCircleAABB(hero,right_platform)){
				if(hero.center.y + hero.velocity.y <= right_platform.center.y - right_platform.half_height() ){
					hero.center.y = right_platform.center.y - right_platform.half_height() - hero.radius;
					hero.on_right_platform = true;
				} else if(hero.center.y + hero.velocity.y > right_platform.center.y + right_platform.half_height() ){
					hero.center.y = right_platform.center.y + right_platform.half_height() - hero.radius;
					hero.velocity.y *= -1;
					hero.on_right_platform= false;
				}
			}
			if(satCircleAABB(hero,top_platform)){
				if(hero.center.y + hero.velocity.y <= top_platform.center.y - top_platform.half_height() ){
					hero.center.y = top_platform.center.y - top_platform.half_height() - hero.radius;
					hero.on_top_platform= true;
				} else if(hero.center.y + hero.velocity.y > top_platform.center.y + top_platform.half_height() ){
					hero.center.y = top_platform.center.y + top_platform.half_height() - hero.radius;
					hero.velocity.y *= -1;
					hero.on_top_platform= false;
				}
			}

			//tube collisions

			//tower collisions
			//tower have life
			//100 for each tower
			//once all towers have been destroyed
			//go to boss battle

        }
        break;
        case BOSS: {
        	//boss to-do-list/pseudocode
        	//lerp boss back and forth
        	//shooting homing missiles which you can destroy
        	//there are only two small platforms
        	//on which you can punch the boss when it is near
        	//colliding self with boss will decrease the life of hero
        }
        break;
        case GAMEOVER: {
            //create the leaderboard
        }
        break; 
    }
    count++; //increment the time between states
}

void SamichIslandApp::draw() {
    //draw menu
    
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	gl::pushMatrices();
	gl::setMatricesWindow (WIND_W, WIND_H);
	gl::setViewport(this->getWindowBounds());
	gl::enableAlphaBlending();
    
    switch( appState.getCurrentState() ) {
		case INIT: {
			string c = boost::lexical_cast<std::string>(count);
			//string t = boost::lexical_cast<std::string>(timeout);    
			//gl::drawString( c + "/" + t, Vec2i( 10, 10 ), Color( 1, 1, 1 ), Font( "Helvetica", 16 ) );
			gl::drawStringCentered( "this mook tower\n", getWindowCenter(), Color (1,1,1), Font ("Helvetica", 36));
			gl::drawStringCentered( c + " PRESS ANY KEY TO CONTINUE" + c, getWindowCenter(), Color (1,1,1), Font ("Helvetica", 12));
		break; }
        case MENU: {
            menuGUI->draw();
        break; }
        case PLAY: {
			int i = 0;
            
            DrawEngine::get().drawSprites();
			//draw punch
			if (hero.punching){
				glColor3f(0, 0, 1);
				gl::drawSolidCircle(punch.center.toV(), punch.radius, 0 );
			}

			//portal from hell
			glColor3f(1,0,1);
            gl::drawSolidRect(createRectangle(portal));
				
			//draw mooks
			for (i = 0; i < cannon_fodder.size(); ++i ) {
				glColor3f(cannon_fodder[i].color.r,cannon_fodder[i].color.g,cannon_fodder[i].color.b);
				Vector2 temp =  lerp( cannon_fodder[i].lerp_time/100 , cannon_fodder[i].prev , cannon_fodder[i].next ) ;
				gl::drawSolidCircle( temp.toV(), cannon_fodder[i].radius ,  0 );
			}

			glColor3f(hero.color.r, hero.color.g, hero.color.b);
			gl::drawSolidCircle(hero.center.toV(), hero.radius, 0 ); //hero
			
			//draw drops
			for (i = 0; i < drops.size(); ++i )
			{
				glColor3f(drops[i].color.r,drops[i].color.g,drops[i].color.b);
				gl::drawSolidCircle(drops[i].center.toV(),drops[i].radius, 0);
			}

			//platforms
			glColor3f(0,1,1);
			gl::drawSolidRect(createRectangle(bottom_platform));
			gl::drawSolidRect(createRectangle(left_platform));
			gl::drawSolidRect(createRectangle(right_platform));
			gl::drawSolidRect(createRectangle(top_platform));

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
            glColor3f(255,0,1);
            gl::drawSolidRect(createRectangle(tower1));
            gl::drawSolidRect(createRectangle(tower2));
            
            string hero_mana = boost::lexical_cast<std::string>(hero.mana);
            string hero_maxmana = boost::lexical_cast<std::string>(hero.maximum_mana);
            string hero_health = boost::lexical_cast<std::string>(hero.health);
            DrawEngine::get().drawString( hero_mana + " of " + hero_maxmana + " mana.", Vector2( 10, 20 ), Color ( 0, 0, 0 ), Font("Helvetica", 16), TEXT_LEFT );
            DrawEngine::get().drawString( hero_health + " of 100", Vector2( 10, 40 ), Color ( 0, 0, 0 ), Font("Helvetica", 16), TEXT_LEFT );
        }
        break;
        case BOSS: {} break;
        case GAMEOVER: {
            gl::drawStringCentered( "GAME OVER!", getWindowCenter(), Color (1,1,1), Font ("Helvetica", 24));            
        }
        break;
	}
	gl::popMatrices();
}

void SamichIslandApp::shutdown() {
    delete menuGUI;
}

CINDER_APP_BASIC( SamichIslandApp, RendererGl )