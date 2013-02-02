#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class IslandStuckApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void IslandStuckApp::setup()
{
}

void IslandStuckApp::mouseDown( MouseEvent event )
{
}

void IslandStuckApp::update()
{
}

void IslandStuckApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( IslandStuckApp, RendererGl )
