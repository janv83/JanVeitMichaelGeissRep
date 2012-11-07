#pragma once
#include "Particle.h"
#include "Character.h"
#include <list>

class ParticleController {
 public:
	ParticleController();
	ParticleController(cinder::Vec2f gravitySource);
	cinder::Vec2f gravitySource;
	std::list<Character> characters;
	void update();
	void draw();
	void addParticles( int amt );
	void removeParticles( int amt );
	void addCharacter();
	void removeCharacter();
	void collisionDetection();
	
	cinder::Vec2f getGravityDirection(cinder::Vec2f location);
	float getGravityAcceleration(cinder::Vec2f location);
	
	std::list<Particle>	mParticles;
};