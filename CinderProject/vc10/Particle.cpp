#include "Particle.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"
//#include <vector>

using namespace ci;
//using namespace app;

Particle::Particle()
{
}

Particle::Particle( Vec2f loc )
{
	mLoc	= loc;
	mVel = Rand::randFloat() * 0.1;
	mRadius	= 10.0;//Rand::randFloat(30.0f);
	color = Rand::randInt(1,4);
}

void Particle::update()
{
	mLoc += mVel*mDir;
	/*if(Rand::randInt(100) > 3)
	{
		mLoc = vector<float>(getWindowSize()/2.0, getWindowSize()/2.0);
	}*/
}

void Particle::draw()
{
	switch(color)
	{
	case 1:
		gl::color(Color(255, 0, 0));
		break;
	case 2:
		gl::color(Color(0, 255, 0));
		break;
	case 3: 
		gl::color(Color(0, 0, 255));
		break;
	case 4:
		gl::color(Color(255, 255, 0));
		break;
	}
	
	//gl::drawSolidCircle( mLoc, mRadius );
	ci::Rectf rect(mLoc.x-mRadius, mLoc.y-mRadius, mLoc.x + mRadius, mLoc.y + mRadius);
	
	gl::drawSolidRect(rect);
}
