#include "Asteroid.h"



Asteroid::Asteroid(RenderWindow* gameWindow, Vector2f asteroidDimensions, int movementAngle, float radius, Texture* texture)
{
	window = gameWindow;
	angle = movementAngle;
	dimensions = asteroidDimensions;
	speed = 20;
	gameObjectType = asteroid;
	gameObjectSize = large;
	circle.setRadius(radius);
	circle.setTexture(texture);

	circle.setOrigin(radius / 2.f, radius / 2.f);
	circle.setRotation(angle);
	
	deleteNextCycle = false;
}

void Asteroid::Update(float dt)
{
	velocity.y = sin((3.14f / 180)*circle.getRotation()) * dt * speed;
	velocity.x = cos((3.14f / 180)*circle.getRotation())* dt * speed;

	position = position - velocity;
	circle.setPosition(position);
}

void Asteroid::Draw()
{
	window->draw(circle);
}

void Asteroid::HandleCollision(GameObject* collider, Sound* shipExplosion)
{
	if (collider->gameObjectType == Type::ship)
	{
		dynamic_cast<Ship*>(collider)->Damage();
		shipExplosion->play();
	}
	else if (collider->gameObjectType == Type::asteroid)
	{
		Vector2f temp = position - collider->position;
		float angle = atan2f(temp.y, temp.x);
		int ballAngle = angle;

		Vector2f c;
		c.x = cosf(circle.getRotation());
		c.y = sinf(circle.getRotation());
		int tempAngle = atan2f(c.y, -c.x);

		Vector2f d;
		d.x = cosf(collider->circle.getRotation());
		d.y = sinf(collider->circle.getRotation());
		int tempAngle2 = atan2f(d.y, -d.x);

		if (this->position.x > collider->position.x)
		{			
			if (this->position.y > collider->position.y)
			{
				tempAngle += 90;
				tempAngle2 -= 90;
				position = Vector2f(position.x + 5, position.y + 5);
				collider->position = Vector2f(collider->position.x - 5, collider->position.y - 5);
			}
			else if (this->position.y < collider->position.y)
			{
				tempAngle += 90;
				tempAngle2 += 90;
				position = Vector2f(position.x + 5, position.y - 5);
				collider->position = Vector2f(collider->position.x - 5, collider->position.y + 5);
			}
		}
		else if (this->position.x < collider->position.x)
		{
			
			if (this->position.y > collider->position.y)
			{
				tempAngle -= 90;
				tempAngle2 -= 90;
				position = Vector2f(position.x - 5, position.y + 5);
				collider->position = Vector2f(collider->position.x + 5, collider->position.y - 5);

			}
			else if (this->position.y < collider->position.y)
			{
				tempAngle -= 90;
				tempAngle2 += 90;
				position = Vector2f(position.x - 5, position.y - 5);
				collider->position = Vector2f(collider->position.x + 5, collider->position.y + 5);
			}
		}
		circle.rotate(tempAngle);
		collider->circle.rotate(tempAngle2);
	}
	else if (collider->gameObjectType == Type::bullet)
	{
		deleteNextCycle = true;
		collider->deleteNextCycle = true;
	}
}

Asteroid::~Asteroid()
{
}
