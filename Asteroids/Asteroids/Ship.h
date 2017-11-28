#pragma once
#include "GameObject.h"

class Ship :
	public GameObject
{
private:
	const int BeginningSpeed = 200;
	const Vector2f defaultPosition = Vector2f(500, 500);

public:
	
	int lives;
	Sound shipMovement;

	Ship(RenderWindow* gameWindow, Vector2f dimensions, Sound* shipThruster);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void HandleCollision(GameObject* collider, Sound* shipExplosion);
	virtual int getRenderBucket()
	{
		return 1;
	}
	virtual Vector2f getCenter()
	{
		return position;
	}
	void Damage();
	~Ship();
};

