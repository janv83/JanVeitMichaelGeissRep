#include "Character.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"



using namespace ci;
using namespace ci::app;
using namespace std;


Character::Character()
{
	location = Vec2f(0.0, 0.0);
	direction = Vec2f(0.0, 0.0);
	acceleration = 0.1;
	attraction = 1.0;
	color = 1;
	velocity = 0.5;
	friction = 0.99;
	imgRadius = 50;
	isMovingForward = false;
	isMovingRight = false;
	isMovingLeft = false;
	isMovingBack = false;
	img = Surface( loadImage( loadAsset( "claudius.png" ) ) );
	imgShown = img;

}

Character::Character(Vec2f loc )
{
	location = loc;
	direction = Vec2f(0.0, 0.0);
	acceleration = 0.1;
	attraction = 1.0;
	color = 1;
	velocity = 0.5;
	friction = 0.99;
	imgRadius = 50;
	isMovingForward = false;
	isMovingRight = false;
	isMovingLeft = false;
	isMovingBack = false;
	img = Surface(loadImage( loadAsset( "claudius.png" ) ) );
	imgShown = img;
}

Vec2f Character::getLocation()
{
	return location;
}
		
/*Character::Character~()
{
}*/
		
void Character::update()
{
	if(isMovingForward)
	{
		showImageBack();

		Vec2f newVec = direction*velocity + Vec2f(0,-acceleration);

		velocity = newVec.length();
		direction = newVec;
		direction.safeNormalize();
		
		

	}
	if(isMovingLeft)
	{
		showImageLeft();
		Vec2f newVec = direction*velocity + Vec2f(-acceleration, 0);

		velocity = newVec.length();
		direction = newVec;
		direction.safeNormalize();
		
	}
	if(isMovingRight)
	{
		showImageRight();
		Vec2f newVec = direction*velocity + Vec2f(acceleration,0);

		velocity = newVec.length();
		direction = newVec;
		direction.safeNormalize();

	}
	if(isMovingBack)
	{
		showImageFront();
		Vec2f newVec = direction*velocity + Vec2f(0,acceleration);

		velocity = newVec.length();
		direction = newVec;
		direction.safeNormalize();
	}

	velocity = velocity * friction;


	location += velocity*direction;
	if(location[0] >= getWindowWidth()-imgRadius || location[0]-imgRadius < 0  )
	{
		location-=velocity*direction;
		direction[0] = -direction[0];
		velocity = velocity * 0.7;
		location+=velocity*direction;
		
	}
	if(location[1] >= getWindowHeight()-imgRadius || location[1]- imgRadius < 0)
	{
		location-=velocity*direction;
		direction[1] = -direction[1];
		velocity = velocity * 0.7;
		location+=velocity*direction;
	}

	
}
		
void Character::draw()
{
	switch(color)
	{
	case 1:
		gl::color(Color(1,0,0));
		break;
	case 2:
		gl::color(Color(0,1,0));
		break;
	case 3:
		gl::color(Color(0,0,1));
		break;
	case 4:
		gl::color(Color(1,1,0));
		break;


	}
	//gl::color(Color(1,0,0));
	gl::draw( gl::Texture(imgShown), Rectf(location.x-imgRadius, location.y -imgRadius, location.x + imgRadius, location.y + imgRadius));
	

	
}

void Character::setColor(unsigned c)
{
	color = c;
}

unsigned Character::getColor()
{
	return color;
}

float Character::getImgRadius()
{
	return imgRadius;
}


void Character::showImageBack()
{
}

void Character::showImageFront()
{
}

void Character::showImageLeft()
{
}

void Character::showImageRight()
{
}

