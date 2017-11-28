#pragma once
#include "GameObject.h"
#include "Ship.h"
#include "Bullet.h"

class Asteroid : public GameObject
{
public:

	Asteroid(RenderWindow* gameWindow, Vector2f asteroidDimensions, int movementAngle);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void HandleCollision(GameObject* collider, Sound* shipExplosion);
	virtual int getRenderBucket()
	{
		return 0;
	}
	virtual Vector2f getCenter()
	{
		return position;
	}
	~Asteroid();
};

