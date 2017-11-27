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

	Ship(RenderWindow* gameWindow, Vector2f dimensions);
	virtual void Update(float dt);
	virtual void Draw();
	virtual void HandleCollision(GameObject* collider);
	void Damage();
	~Ship();
};

