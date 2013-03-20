#include "Headers.h"
#include "SamichIslandApp.h"

using namespace ci;
using namespace ci::app;
using namespace std;

float game_time;
float curr_time, prev_time;
const int TIME_UP = 3;

float shoot_time, punch_time;
float dash_time, dash_accel, dash_limit, punch_delay;

int j = 0; //enemy count
const int MAX_MOOK = 50;

int dash_mana_cost;
Vector2 mouse;

void SamichIslandApp::prepareSettings( Settings *settings ) {
	settings->setWindowSize( 800, 600 );
	settings->setFrameRate( 60.0f );
	settings->setWindowPos(10,10);
	settings->setResizable( false );
    settings->setTitle("this mook thing");
}

void SamichIslandApp::resize(ResizeEvent event) {
	mCam.setPerspective( 45, getWindowAspectRatio(), 1, 100 );
	gl::setMatrices( mCam );
}

void SamichIslandApp::setup(){
    yor_score = 0;
    hi_score = 0;

    //drawengine setup
    dg = new DrawEngine();
    dg->updateBounds(getWindowBounds());
    dg->setBackgroundPath("ikabg.bmp");
    dg->setSpritesheetPath("spritesheet.png");
    
    //menu setup
    menuGUI = new ciUICanvas(0,0,getWindowWidth(),getWindowHeight());
    menuGUI->addWidgetDown(new ciUILabelButton(getWindowWidth()-CI_UI_GLOBAL_WIDGET_SPACING*2, false, "PLAY", CI_UI_FONT_LARGE));
    menuGUI->addWidgetDown(new ciUILabelButton(getWindowWidth()-CI_UI_GLOBAL_WIDGET_SPACING*2, false, "LEADERBOARDS", CI_UI_FONT_LARGE));
    menuGUI->addWidgetDown(new ciUILabelButton(getWindowWidth()-CI_UI_GLOBAL_WIDGET_SPACING*2, false, "EXIT", CI_UI_FONT_LARGE));
    menuGUI->autoSizeToFitWidgets();
    menuGUI->registerUIEvents(this, &SamichIslandApp::guiEvent);

    //drawengine setup
	appState.setNextState( INIT );

	//time initializations
	game_time = shoot_time = dash_time = punch_time = 0;

	//window bluh//
	WIND_H = this->getWindowHeight();
	WIND_W = this->getWindowWidth();
    
	//hero initialization
    hero.name = "HERO";
    hero.width = 45;
    hero.height = 45;
    hero.health = 100;
    hero.mana = 100;
    hero.maximum_mana = 100;
	hero.center = Vector2(WIND_W/2, WIND_H - hero.half_height());
	hero.color = Colorf(1.0,1.0,1.0);
	hero.velocity.x = 8.0;
	hero.velocity.y = 8.0;
	hero.moving = hero.jumping = hero.dashing = hero.punching 
	= hero.jumpKey = hero.leftKey = hero.rightKey = hero.dashKey
	= hero.needsGravity = false;
	hero.on_btm_platform = hero.on_left_platform = hero.on_right_platform = hero.on_top_platform = false;
	JUMP_H = 16;
	hero.damage = 10;
    
	//punch initialization
	punch.radius = 10;
	punch.center = hero.center;
	punch.center.x += hero.half_width();
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
    portal.name = "PORTAL";

	//platform things
	bottom_platform.center = Vector2( WIND_W/2 , 500 );
	bottom_platform.height = 40;
	bottom_platform.width = 300;
    bottom_platform.name = "BOTTOM_PLATFORM";
	top_platform.center = Vector2( WIND_W/2 , 240 );
	top_platform.height = 40;
	top_platform.width = 300;
    top_platform.name = "TOP_PLATFORM";
	left_platform.center = Vector2( WIND_W*2/20 , 360 );
	left_platform.height = 40;
	left_platform.width = 300;
    left_platform.name = "LEFT_PLATFORM";
	right_platform.center = Vector2( WIND_W*18/20 , 360 );
	right_platform.height = 40;
	right_platform.width = 300;
    right_platform.name = "RIGHT_PLATFORM";

	//tower targets
    tower1.center = Vector2( 20, 50 );
    tower1.height = 70;
    tower1.width = 30;
    tower1.initHealth(100);
    tower1.name = "TOWER_1";
    tower2.center = Vector2( WIND_W - 20 , 50 );
    tower2.height = 70;
    tower2.width = 30;
    tower2.initHealth(100);
    tower2.name = "TOWER_2";

    //tower guard
    towerguard1.center = Vector2(tower1.center.x + 30, tower1.center.y + 40);
    towerguard1.height = 20;
    towerguard1.width = 90;
    towerguard1.name = "TOWERGUARD1";
    towerguard2.center = Vector2(tower2.center.x - 30, tower2.center.y + 40);
    towerguard2.height = 20;
    towerguard2.width = 90;
    towerguard2.name = "TOWERGUARD2";

	//tubes
	tubeA.height = 100;
	tubeA.width = 100;
	tubeA.center = Vector2( tubeA.half_width() - 20 , WIND_H-(tubeA.half_height()) );
    tubeA.name = "TUBE_A";
	tubeB.height = 100;
	tubeB.width = 100;
	tubeB.center = Vector2( WIND_W-tubeB.half_width() + 20 , WIND_H-tubeB.half_height() );
    tubeB.name = "TUBE_B";
    
    //ANIMATION/SPRITE STUFF
    //How to use inefficient DrawEngine
    //#1 create animation object
    //new Animation (name, width, height, the rate in which frames change )
    //#2 addSprite( new Sprite(name of sprite, location in the spritesheet) )
    //#3 add the animation object to drawEngine
    //#4 updatePosition
    //in this case, the tube does not move. so no need to call this on update method
    //#5 if your object moves updatePosition in update() method
    //IMMOVABLE OBJECTS!!!!!!
    //tube A
    tubeA_anim = new Animation(tubeA.name, 100, 100, 0);// 0 for now
    tubeA_anim->addSprite(new Sprite(tubeA.name, Vector2(476,2))); //position on the sprite sheet not the AABB itself
    dg->addAnimations(tubeA_anim);
    dg->updatePositions(tubeA_anim->getAnimName(), tubeA);
    
    //tube B
    tubeB_anim = new Animation(tubeB.name, 100, 100, 0);// 0 for now
    tubeB_anim->addSprite(new Sprite(tubeB.name, Vector2(578,2))); //position on the sprite sheet not the AABB itself
    dg->addAnimations(tubeB_anim);
    dg->updatePositions(tubeB_anim->getAnimName(), tubeB);
    
    //portal
    portal_anim = new Animation(portal.name, 36, 36, 0);
    portal_anim->addSprite(new Sprite(portal.name, Vector2(754,104)));
    dg->addAnimations(portal_anim);
    dg->updatePositions(portal_anim->getAnimName(), portal);
    
    //platforms
    bottom_platform_anim = new Animation(bottom_platform.name, 121, 16, 0);
    bottom_platform_anim->addSprite(new Sprite(bottom_platform.name, Vector2(680,2)) );
    dg->addAnimations(bottom_platform_anim);
    dg->updatePositions(bottom_platform_anim->getAnimName(), bottom_platform);
    
    top_platform_anim = new Animation(top_platform.name, 121, 16, 0);
    top_platform_anim->addSprite(new Sprite(top_platform.name, Vector2(680,2)) );
    dg->addAnimations(top_platform_anim);
    dg->updatePositions(top_platform_anim->getAnimName(), top_platform);
    
    right_platform_anim = new Animation(right_platform.name, 121, 16, 0);
    right_platform_anim->addSprite(new Sprite(right_platform.name, Vector2(680,2)) );
    dg->addAnimations(right_platform_anim);
    dg->updatePositions(right_platform_anim->getAnimName(), right_platform);
    
    left_platform_anim = new Animation(left_platform.name, 121, 16, 0);
    left_platform_anim->addSprite(new Sprite(left_platform.name, Vector2(680,2)) );
    dg->addAnimations(left_platform_anim);
    dg->updatePositions(left_platform_anim->getAnimName(), left_platform);
    
    //towers
    tower1_anim = new Animation(tower1.name, 21, 36, 0);
    tower1_anim->addSprite(new Sprite(tower1.name, Vector2(792,104) ));
    dg->addAnimations(tower1_anim);
    dg->updatePositions(tower1_anim->getAnimName(), tower1); 
    
    tower2_anim = new Animation(tower2.name, 21, 36, 0);
    tower2_anim->addSprite(new Sprite(tower2.name, Vector2(792,104) ));
    dg->addAnimations(tower2_anim);
    dg->updatePositions(tower2_anim->getAnimName(), tower2);
    
    //towerguards
    towerguard1_anim = new Animation(towerguard1.name, 121, 16, 0);
    towerguard1_anim->addSprite(new Sprite(towerguard1.name, Vector2(680,2)) );
    dg->addAnimations(towerguard1_anim);
    dg->updatePositions(towerguard1_anim->getAnimName(), towerguard1);
    
    towerguard2_anim = new Animation(towerguard2.name, 121, 16, 0);
    towerguard2_anim->addSprite(new Sprite(towerguard2.name, Vector2(680,2)) );
    dg->addAnimations(towerguard2_anim);
    dg->updatePositions(towerguard2_anim->getAnimName(), towerguard2);
    
    //hero
    hero_anim = new Animation(hero.name, 32, 32, 10); //will change sprite every 15 frames
    hero_anim->addSprite(new Sprite(hero.name, Vector2(100, 104))); //add frames here
    dg->addAnimations(hero_anim);
    dg->updatePositions(hero_anim->getAnimName(), hero);

    //mook
    for(int i = 0; i < MAX_MOOK; ++i){
		Mook mook;
        mook.id = i;
        mook.exists = false;
		mook.health = 100;
		mook.radius = 30;
		mook.center = portal.center;
		cannon_fodder.push_back(mook);
        // mook_anim[i] = new Animation(cannon_fodder[i].name + i, 45, 45, 10 );
        // mook_anim[i]->addSprite( new Sprite() );
        for(int j = 0; j < 3; ++j){
           cannon_fodder[i].list.push_back(Vector2( (float)rand()/RAND_MAX * this->getWindowWidth() 
           , (float)rand()/RAND_MAX * this->getWindowHeight()) );
        }
        // cannon_fodder[i].prev.x = (float)rand()/RAND_MAX * this->getWindowWidth();
        // cannon_fodder[i].prev.y = (float)rand()/RAND_MAX * this->getWindowHeight();
        // cannon_fodder[i].next = Vector2( this->getWindowWidth()/2 , this->getWindowHeight() );
        cannon_fodder[i].lerp_time = 0;

        float temp = ( -1+2*((float)rand())/RAND_MAX );
        if(temp < 0) 
            cannon_fodder[i].direction = -1;
        else cannon_fodder[i].direction = 1;
        console() << "mook_id=" << mook.id << std::endl;
	}
}

void SamichIslandApp::keyDown( KeyEvent event ) {
	switch(appState.getCurrentState()){
		case INIT: 
			appState.setNextState( MENU );
		break;
        case GAMEOVER: 
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
						punch_time = shoot_time;
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
				// case KeyEvent::KEY_F12:
		  //       case KeyEvent::KEY_f:
				// 	setFullScreen( ! isFullScreen() );
				// break;
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
    string t = boost::lexical_cast<std::string>(curr_time);
    string v = boost::lexical_cast<std::string>(game_time);
    console() << "curr_time=" << t << std::endl;
    console() << "game_time=" << v << std::endl;
    console() << "gemtim*10=" << j << std::endl;

    j = (int)(game_time*10);

    //draw engine updates
    dg->updateBounds(getWindowBounds());
	//state management updates
	bool change = appState.commitState();
    if (change) count = 0;
    
    switch( appState.getCurrentState() ) {
        case INIT:
			//bluh
            prev_time = 0;
            curr_time = 0;
            game_time = 0;
        break;
        case MENU: {
            //menu update
            menuGUI->update();
            prev_time = 0;
            curr_time = 0;
            game_time = 0;
        }
        case PLAY: {
            prev_time = ci::app::getElapsedSeconds();

//**=======================================EVERYTHING RELATED TO HERO============================================**//
            
            //limit the hero to game boundaries
            if (hero.center.x - hero.velocity.x <= 0) {
                hero.center.x = hero.half_width();
            }
            else if (hero.center.x + hero.velocity.x >= WIND_W ) {
                hero.center.x = WIND_W - hero.half_width();
            }
            
            //punch
            shoot_time = ci::app::getElapsedSeconds() * 1000;
			if (hero.punching) {
				int dir = -1;
				if (punch.isRight) dir = 1;
				if (punch_time == 0) punch_time = shoot_time;

				float secs = (shoot_time - punch_time)/1000;
				if (secs >= 0.156) dir *= -1;
				if (secs >= 0.313) {
					hero.punching = false;
					punch.center = hero.center;
					punch.center.x += -1*dir*hero.half_width();
                    punch.center.y = hero.center.y;
					punch_time = 0;
				} else {
                    punch.center.x += dir*((((9*secs)-1.4)*((9*secs)-1.4))+2);
                    punch.center.y = hero.center.y;
                }
			} else punch.center = hero.center;
				
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
					hero.velocity.y -= 1;
				} else {
					hero.jumping = false;
                    hero.center.y = WIND_H - hero.half_height(); //APPLEGUST
				}
			}

			//jump from platforms			
			if( hero.jumping == true && hero.on_btm_platform == true) {
				if( (hero.center.y - hero.velocity.y) <= bottom_platform.center.y - bottom_platform.half_height()) {
					hero.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else
                    hero.jumping = false;
                
			}
			if( hero.jumping == true && hero.on_left_platform == true) {
				if ( (hero.center.y - hero.velocity.y) <= left_platform.center.y - left_platform.half_height() ) {
					hero.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;
			}
			if( hero.jumping == true && hero.on_right_platform == true) {
				if( (hero.center.y - hero.velocity.y) <= right_platform.center.y - right_platform.half_height()) {
					hero.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;
			}
			if( hero.jumping == true && hero.on_top_platform == true) {
				if ( (hero.center.y - hero.velocity.y) <= top_platform.center.y - top_platform.half_height() ) {
					hero.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else hero.jumping = false;
			}

			//hacky code for missing gravities
			if( hero.on_btm_platform == false && hero.on_left_platform == false && hero.on_right_platform == false && hero.on_top_platform == false
				&& hero.needsGravity == true ) {
				if ( (hero.center.y - hero.velocity.y) <= WIND_H  ) {
					hero.center.y -= hero.velocity.y;
					hero.velocity.y -= 1;
				} else {
					hero.needsGravity = false;
                    hero.center.y = WIND_H - hero.half_height();
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
            //if (count % 180 == 0 ) hero.regenerateMana(1);
            
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

            console() << "amIpunch=" << hero.punching << std::endl;

            for(int i = 0; i < cannon_fodder.size(); ++i){
                if(i == (int)(game_time*10))
                    cannon_fodder[i].exists = true;
                if(cannon_fodder[i].exists){
                    cannon_fodder[i].lerp_time += 0.1f;
                    if( cannon_fodder[i].lerp_time > 100 ) {
                        cannon_fodder[i].lerp_time = 0;
                        // cannon_fodder[i].prev.x = (float)rand()/RAND_MAX * this->getWindowWidth();
                        // cannon_fodder[i].prev.y = (float)rand()/RAND_MAX * this->getWindowHeight();
                    }
                }
                // console() << "hit="<<satCircleAABB(cannon_fodder[i], hero) << std::endl;
                // if(satCircleAABB(cannon_fodder[i], hero)){
                //     // cannon_fodder[i].exists = false;
                //     yor_score += 100;
                   
                // }
            }
				
//**==================================EVERYTHING NOT RELATED TO HERO OR MOOK===================================**//

			//platform collisions
			if(satAABB(hero,bottom_platform)){
				if(hero.center.y + hero.velocity.y <= bottom_platform.center.y - bottom_platform.half_height() ){
					hero.center.y = bottom_platform.center.y - bottom_platform.half_height() - hero.half_height();
					hero.on_btm_platform = true;
				} else if(hero.center.y + hero.velocity.y > bottom_platform.center.y + bottom_platform.half_height() ){
					hero.center.y = bottom_platform.center.y + bottom_platform.half_height() + hero.half_height();
					hero.velocity.y *= -1;
					hero.on_btm_platform = false;
				}
			}
			if(satAABB(hero,left_platform)){
				if(hero.center.y + hero.velocity.y <= left_platform.center.y - left_platform.half_height() ){
					hero.center.y = left_platform.center.y - left_platform.half_height() - hero.half_height();
					hero.on_left_platform = true;
				} else if(hero.center.y + hero.velocity.y > left_platform.center.y + left_platform.half_height() ){
					hero.center.y = left_platform.center.y + left_platform.half_height() + hero.half_height();
					hero.velocity.y *= -1;
					hero.on_left_platform = false;
				}
			}
			if(satAABB(hero,right_platform)){
				if(hero.center.y + hero.velocity.y <= right_platform.center.y - right_platform.half_height() ){
					hero.center.y = right_platform.center.y - right_platform.half_height() - hero.half_height();
					hero.on_right_platform = true;
				} else if(hero.center.y + hero.velocity.y > right_platform.center.y + right_platform.half_height() ){
					hero.center.y = right_platform.center.y + right_platform.half_height() + hero.half_height();
					hero.velocity.y *= -1;
					hero.on_right_platform= false;
				}
			}
			if(satAABB(hero,top_platform)){
				if(hero.center.y + hero.velocity.y <= top_platform.center.y - top_platform.half_height() ){
					hero.center.y = top_platform.center.y - top_platform.half_height() - hero.half_height();
					hero.on_top_platform= true;
				} else if(hero.center.y + hero.velocity.y > top_platform.center.y + top_platform.half_height() ){
					hero.center.y = top_platform.center.y + top_platform.half_height() + hero.half_height();
					hero.velocity.y *= -1;
					hero.on_top_platform= false;
				}
			} 

            //update mook and hero coords in drawengine
            dg->updatePositions(hero_anim->getAnimName(), hero);

            //timer instead of whatever else we planned
            curr_time = (ci::app::getElapsedSeconds()) - prev_time;

                if(curr_time > 0.016f)
                    curr_time = 0.016f;
                if(curr_time < 0.001f)
                    curr_time = 0.001f;
            
            game_time += curr_time;

            if(game_time >= TIME_UP)
                appState.setNextState(GAMEOVER);
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
            //reset game time
            prev_time = 0;
            curr_time = 0;
            game_time = 0;
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
			gl::drawStringCentered( "this mook thing\n", getWindowCenter(), Color (1,1,1), Font ("Helvetica", 36));
			gl::drawStringCentered( c + " PRESS ANY KEY TO CONTINUE" + c, getWindowCenter(), Color (1,1,1), Font ("Helvetica", 12));
		break; }
        case MENU: {
            menuGUI->draw();
            string c = boost::lexical_cast<std::string>(count);
            gl::drawStringCentered( "this mook thing\n", Vec2f(getWindowWidth()/2, (getWindowHeight()/2)) , Color (1,1,1), Font ("Helvetica", 48));
            gl::drawStringCentered( c + "=========================================================="+ c, Vec2f(getWindowWidth()/2, (getWindowHeight()/2) + 20), Color (1,1,1), Font ("Helvetica", 12));
        break; }
        case PLAY: {
			int i = 0;
            //draw background here
            dg->drawBackground();

			//glColor3f(hero.color.r, hero.color.g, hero.color.b);
            //gl::drawSolidRect(createRectangle(hero));
			//gl::drawSolidCircle(hero.center.toV(), hero.radius, 0 ); //hero

			//portal from hell
			//glColor3f(1,0,1);
            //gl::drawSolidRect(createRectangle(portal));
			dg->drawAnimation(portal_anim);
            
			//draw mooks
			for (i = 0; i < cannon_fodder.size(); ++i ) {
                if(cannon_fodder[i].exists){
                    glColor3f(cannon_fodder[i].color.r,cannon_fodder[i].color.g,cannon_fodder[i].color.b);
                    // Vector2 temp =  lerp( cannon_fodder[i].lerp_time/100 , cannon_fodder[i].prev , cannon_fodder[i].next ) ;
                    Vector2 temp =  generalLerp( cannon_fodder[i].lerp_time/100 , cannon_fodder[i].list) ;
                    gl::drawSolidCircle( temp.toV(), cannon_fodder[i].radius , 0 );
                    //gl::drawSolidCircle(cannon_fodder[i].prev.toV() , cannon_fodder[i].radius , 0 );                    
                }
			}

			//draw drops
			// for (i = 0; i < drops.size(); ++i )
			// {
			// 	glColor3f(drops[i].color.r,drops[i].color.g,drops[i].color.b);
			// 	gl::drawSolidCircle(drops[i].center.toV(),drops[i].radius, 0);
			// }

			//platforms
			//glColor3f(0,1,1);
			//gl::drawSolidRect(createRectangle(bottom_platform));
			//gl::drawSolidRect(createRectangle(left_platform));
			//gl::drawSolidRect(createRectangle(right_platform));
			//gl::drawSolidRect(createRectangle(top_platform));
            dg->drawAnimation(bottom_platform_anim);
            dg->drawAnimation(top_platform_anim);
            dg->drawAnimation(right_platform_anim);
            dg->drawAnimation(left_platform_anim);
            
            
			//draw bullets
			// for(; i < dakka.size(); ++i) {
			// 	glColor3f(1, 0, 0);
			// 	gl::drawSolidCircle(dakka[i].center.toV(), dakka[i].radius, 0 );
			// }

			//tubes
			glColor3f(1,1,0);
            dg->drawAnimation(tubeA_anim);
            dg->drawAnimation(tubeB_anim);
			//gl::drawSolidRect(createRectangle(tubeA));
			//gl::drawSolidRect(createRectangle(tubeB));
            
            //tower
            //glColor3f(255,0,1);
            //gl::drawSolidRect(createRectangle(tower1));
            //gl::drawSolidRect(createRectangle(tower2));
            //glColor3f(20, 20, 20);
            //gl::drawSolidRect(createRectangle(towerguard1));
            //gl::drawSolidRect(createRectangle(towerguard2));
            
            dg->drawAnimation(tower1_anim);
            dg->drawAnimation(tower2_anim);
            
            dg->drawAnimation(towerguard1_anim);
            dg->drawAnimation(towerguard2_anim);
            
            // string hero_mana = boost::lexical_cast<std::string>(hero.mana);
            // string hero_maxmana = boost::lexical_cast<std::string>(hero.maximum_mana);
            // string hero_health = boost::lexical_cast<std::string>(hero.health);

            // string timer = boost::lexical_cast<std::string> (TIME_UP - game_time) ;
            // console() << "gemtim*10=" << (int)(game_time*10) << std::endl;
            string timer = boost::lexical_cast<std::string> (TIME_UP*10 - (int)(game_time*10)) ;

            // gl::drawString( hero_mana + " of " + hero_maxmana + " mana.", Vector2( 10, 20 ).toV(), Color ( 0, 0, 0 ), Font("Helvetica", 16));
            gl::drawStringCentered( timer , Vec2f(portal.center.x+2, portal.center.y-35) , Color ( 0, 0, 0 ), Font("Arial Black", 48));

            //draw punch
            if (hero.punching){
                glColor3f(0, 0, 1);
                gl::drawSolidCircle(punch.center.toV(), punch.radius, 0 );
            }
            dg->drawAnimation(hero_anim);
        }
        break;
        case BOSS: {} break;
        case GAMEOVER: {
            string your_score = boost::lexical_cast<std::string> (yor_score);
            string high_score = boost::lexical_cast<std::string> (hi_score);
            gl::drawStringCentered( "times up!", getWindowCenter(), Color (1,1,1), Font ("Helvetica", 36)); 
            gl::drawStringCentered( "your score: " + your_score, Vec2f(getWindowWidth()/2, (getWindowHeight()/2) + 40) , Color (1,1,1), Font ("Helvetica", 24));    
            gl::drawStringCentered( "high score: " + high_score, Vec2f(getWindowWidth()/2, (getWindowHeight()/2) + 60) , Color (1,1,1), Font ("Helvetica", 24));    

            for(int i = 0; i < cannon_fodder.size(); ++i){
                cannon_fodder[i].exists = false;
            }
        }
        break;
	}
	gl::popMatrices();
}

void SamichIslandApp::shutdown() {
    delete menuGUI;
}

CINDER_APP_BASIC( SamichIslandApp, RendererGl )