#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:

	Bullet(RenderWindow* gameWindow, Vector2f bulletDimensions, int bulletAngle);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void HandleCollision(GameObject* collider, Sound* shipExplosion);
	~Bullet();
};

