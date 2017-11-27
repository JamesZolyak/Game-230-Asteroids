#pragma once
#include "GameObject.h"
#include "Ship.h"
#include "Bullet.h"

class Asteroid : public GameObject
{
public:

	int radius;

	Asteroid(RenderWindow* gameWindow, Vector2f asteroidDimensions, int movementAngle);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void HandleCollision(GameObject* collider);
	~Asteroid();
};
