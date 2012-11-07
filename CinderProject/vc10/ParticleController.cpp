#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "ParticleController.h"
#include <vector>
using namespace ci;
using std::list;

ParticleController::ParticleController()
{
}

ParticleController::ParticleController(Vec2f gravitySource)
{
	ParticleController::gravitySource = gravitySource;
}

void ParticleController::update()
{
	for( list<Character>::iterator c = characters.begin(); c != characters.end(); ++c )
	{
		c->update();
	}

	gravitySource = characters.front().getLocation();
	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		for( list<Character>::iterator c = characters.begin(); c != characters.end(); ++c )
		{
			if(p->color == c->getColor())
			{
				gravitySource = c->getLocation();
		
				Vec2f distance  = gravitySource - p->mLoc;
				if(distance.lengthSquared() >400.0)
				{
				p->mDir += getGravityDirection(p->mLoc);
				p->mDir.safeNormalize();
				p->mVel = p->mRadius * getGravityAcceleration(p->mLoc);
				}
				else
				{
					//p->mDir = Vec2f(0.0,0.0);
					
					p->mDir += getGravityDirection(p->mLoc);
					p->mDir.safeNormalize();
					p->mVel = p->mRadius * 0.03;
				}
			}
		}
		collisionDetection();
		p->update();


	}
}

void ParticleController::draw()
{
	for( list<Character>::iterator c = characters.begin(); c != characters.end(); ++c )
	{
		c->draw();
	}

	for( list<Particle>::iterator p = mParticles.begin(); p != mParticles.end(); ++p ){
		
		

		p->draw();
	}
}

void ParticleController::addParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		float x = Rand::randFloat( app::getWindowWidth() );
		float y = Rand::randFloat( app::getWindowHeight() );
		mParticles.push_back( Particle( Vec2f( x, y ) ) );
	}
}

void ParticleController::addCharacter()
{
	float x = Rand::randFloat( app::getWindowWidth() );
	float y = Rand::randFloat( app::getWindowHeight() );
	characters.push_back(Character(Vec2f(x,y)));
}

void ParticleController::removeCharacter()
{
	characters.pop_back();
}

void ParticleController::removeParticles( int amt )
{
	for( int i=0; i<amt; i++ )
	{
		mParticles.pop_back();
	}
}

Vec2f ParticleController::getGravityDirection(Vec2f location)
{
	Vec2f gravityVector = gravitySource - location;
	gravityVector.safeNormalize();
	
	return gravityVector;
}

float ParticleController::getGravityAcceleration(Vec2f location)
{
	Vec2f gravityVector = gravitySource - location;
	/*if(gravityVector.length() < 1)
	{

		return 9.81;
	}
	else
	{
		return 9.81*(1/gravityVector.lengthSquared());
	}*/

	if(gravityVector.lengthSquared() < 40000)
	{
		return 0.06;
	}
	else 
	{
		return 0;
	}

	
}

void ParticleController::collisionDetection()
{
	for( list<Particle>::iterator p1 = mParticles.begin(); p1 != mParticles.end(); ++p1 )
	{
		list<Particle>::iterator p2 = p1;
		
		
			
			for(++p2; p2 != mParticles.end(); ++p2 )
			{
				if(p2->mVel > 0 || p1->mVel > 0)   // nur punkte prüfen die sich bewegen
				{
					float distanceX = p2->mLoc.x - p1->mLoc.x;
					distanceX = fabs(distanceX);

					float distanceY = p2->mLoc.y - p1->mLoc.y;
					distanceY = fabs(distanceY);

					if(distanceX <= p1->mRadius+p2->mRadius && distanceY <= p1->mRadius + p2->mRadius) 
					{
						p2->mDir = -p2->mDir;  
						p1->mDir = -p1->mDir;
						//std::cout << "collision " << std::endl;

						p2->mVel = p2->mVel * 0.3;
						p1->mVel = p1->mVel * 0.3;
					}
				}

				

	
			}

			for(list<Character>::iterator c = characters.begin(); c != characters.end(); ++c)
			{
				if(fabs(p1->mLoc.x - c->getLocation().x) < p1->mRadius + c->getImgRadius() && fabs(p1->mLoc.y - c->getLocation().y) < p1->mRadius + c->getImgRadius())
				{

					//collision between character and p1
					p1->mDir = -p1->mDir;
					p1->mVel = 0.3 * p1->mVel;
				}
			}
		

		
	}
}
