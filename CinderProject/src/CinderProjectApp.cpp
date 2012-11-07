#include "ParticleController.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "Character.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class CinderProjectApp : public AppBasic {
  public:
	gl::Texture myImage;
	ParticleController* particleController;
	Vec2f gravitySource;
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings *settings);
	void keyDown(KeyEvent event);
	void keyUp( KeyEvent event );
};

void CinderProjectApp::setup()
{
	myImage = gl::Texture( loadImage( loadAsset( "bg.png" ) ) );
	
	
	
	
	particleController =  new ParticleController(gravitySource);
	particleController->addCharacter();
	particleController->addCharacter();

	particleController->addParticles(20);
}

void CinderProjectApp::mouseDown( MouseEvent event )
{
}

void CinderProjectApp::update()
{
	
	
	particleController->update();
	
}

void CinderProjectApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
	//float gray = sin( getElapsedSeconds() ) * 0.5f + 0.5f;
	//gl::clear( Color( gray, gray, gray ), true );
	//gl::draw( myImage, getWindowBounds() );

	/*float x = cos( getElapsedSeconds() ) * 100.0f;
	float y = sin( getElapsedSeconds() ) * 100.0f;
	gl::drawSolidCircle( Vec2f( x, y ) + getWindowSize() / 2, 50.0f );
	gl::drawSolidCircle( Vec2f( x, y ) + getWindowSize() / 2, abs( x ) );
	*/
	
	particleController->draw();

	
}

void CinderProjectApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 1200, 800 );
    settings->setFrameRate( 60.0f );
}

void CinderProjectApp::keyDown( KeyEvent event ) {


	//player 1
    if( event.getChar() == 'w' )
	{
		particleController->characters.front().isMovingForward = true;
    } 
	else if( event.getChar() == 'a' )
	{
       particleController->characters.front().isMovingLeft = true;
    }
	else if( event.getChar() == 'd' )
	{
       particleController->characters.front().isMovingRight = true;
    }
	else if( event.getChar() == 's' )
	{
       particleController->characters.front().isMovingBack = true;
    }
	else if(event.getChar() =='1')
	{
		particleController->characters.front().setColor(1);
	}
	else if(event.getChar() =='2')
	{
		particleController->characters.front().setColor(2);
	}
	else if(event.getChar() =='3')
	{
		particleController->characters.front().setColor(3);
	}
	else if(event.getChar() =='4')
	{
		particleController->characters.front().setColor(4);
	}
	//player 2
	else if( event.getCode() == KeyEvent::KEY_UP)
	{
		particleController->characters.back().isMovingForward = true;
    } 
	else if( event.getCode() == KeyEvent::KEY_LEFT )
	{
       particleController->characters.back().isMovingLeft = true;
    }
	else if( event.getCode() == KeyEvent::KEY_RIGHT )
	{
       particleController->characters.back().isMovingRight = true;
    }
	else if( event.getCode() == KeyEvent::KEY_DOWN )
	{
       particleController->characters.back().isMovingBack = true;
    }
	else if(event.getChar() =='9')
	{
		particleController->characters.back().setColor(1);
	}
	else if(event.getChar() =='8')
	{
		particleController->characters.back().setColor(2);
	}
	else if(event.getChar() =='7')
	{
		particleController->characters.back().setColor(3);
	}
	else if(event.getChar() =='6')
	{
		particleController->characters.back().setColor(4);
	}

}

void CinderProjectApp::keyUp( KeyEvent event ) {
    if( event.getChar() == 'w' )
	{
		particleController->characters.front().isMovingForward = false;
    } 
	else if( event.getChar() == 'a' )
	{
       particleController->characters.front().isMovingLeft =false;
    }
	else if( event.getChar() == 'd' )
	{
       particleController->characters.front().isMovingRight = false;
    }
	else if( event.getChar() == 's' )
	{
       particleController->characters.front().isMovingBack = false;
    }
	//player 2
	else if( event.getCode() == KeyEvent::KEY_UP)
	{
		particleController->characters.back().isMovingForward = false;
    } 
	else if( event.getCode() == KeyEvent::KEY_LEFT )
	{
       particleController->characters.back().isMovingLeft = false;
    }
	else if( event.getCode() == KeyEvent::KEY_RIGHT )
	{
       particleController->characters.back().isMovingRight = false;
    }
	else if( event.getCode() == KeyEvent::KEY_DOWN )
	{
       particleController->characters.back().isMovingBack = false;
    }
}



CINDER_APP_BASIC( CinderProjectApp, RendererGl )
