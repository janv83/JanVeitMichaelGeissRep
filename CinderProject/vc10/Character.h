#ifndef CHARACTER_H
#define CHARACTER_H
#include "cinder/Vector.h"
#include "cinder/Surface.h"



class Character
{
	public: 
		Character();
		Character(cinder::Vec2f);
		//Character~();
		
		void update();
		void draw();
		cinder::Vec2f getLocation();
		unsigned getColor();
		void setColor(unsigned c);
		float getImgRadius();

		bool isMovingForward;
		bool isMovingBack;
		bool isMovingLeft;
		bool isMovingRight;
	private:
		cinder::Vec2f location;
		float acceleration;
		float attraction;
		float friction;
		float velocity;
		float imgRadius;
		unsigned color;
		cinder::Vec2f direction;

		
		cinder::Surface img;
		cinder::Surface imgShown;

		void showImageFront();
		void showImageBack();
		void showImageLeft();
		void showImageRight();

};




#endif