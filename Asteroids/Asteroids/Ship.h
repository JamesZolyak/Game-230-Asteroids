#pragma once
#include "GameObject.h"

class Ship :
	public GameObject
{
private:
	const int BeginningSpeed = 200;
	const Vector2f defaultPosition = Vector2f(400, 300);

public:
	
	int lives;
	Sound shipMovement;

	Ship(RenderWindow* gameWindow, Vector2f dimensions, Sound* shipThruster);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void HandleCollision(GameObject* collider, Sound* shipExplosion);
	void Damage();
	~Ship();
};

