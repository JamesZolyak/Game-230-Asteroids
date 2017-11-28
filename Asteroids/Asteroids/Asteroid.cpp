#include "Asteroid.h"



Asteroid::Asteroid(RenderWindow* gameWindow, Vector2f asteroidDimensions, int movementAngle)
{
	window = gameWindow;
	angle = movementAngle;
	dimensions = asteroidDimensions;
	speed = 20;
	gameObjectType = asteroid;
	gameObjectSize = large;
	shape.setSize(dimensions - sf::Vector2f(3, 3));
	shape.setFillColor(sf::Color::White);
	shape.setOrigin(dimensions / 2.f);
	shape.setRotation(angle);
	deleteNextCycle = false;
}

void Asteroid::Update(float dt)
{
	velocity.y = sin((3.14f / 180)*shape.getRotation()) * dt * speed;
	velocity.x = cos((3.14f / 180)*shape.getRotation())* dt * speed;

	position = position - velocity;
	shape.setPosition(position);
}

void Asteroid::Draw()
{
	window->draw(shape);
}

void Asteroid::HandleCollision(GameObject* collider, Sound* shipExplosion)
{
	if(shape.getGlobalBounds().intersects(collider->shape.getGlobalBounds()))
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
			c.x = cosf(shape.getRotation());
			c.y = sinf(shape.getRotation());
			int tempAngle = atan2f(c.y, -c.x);

			Vector2f d;
			d.x = cosf(collider->shape.getRotation());
			d.y = sinf(collider->shape.getRotation());
			int tempAngle2 = atan2f(d.y, -d.x);

			if (this->position.x > collider->position.x)
			{
				tempAngle += 90;
				tempAngle2 -= 90;
				if (this->position.y > collider->position.y)
				{
					position = Vector2f(position.x + 5, position.y + 5);
					collider->position = Vector2f(collider->position.x - 5, collider->position.y - 5);
				}
				else if (this->position.y < collider->position.y)
				{
					position = Vector2f(position.x + 5, position.y - 5);
					collider->position = Vector2f(collider->position.x - 5, collider->position.y + 5);
				}
			}
			else if (this->position.x < collider->position.x)
			{
				tempAngle -= 90;
				tempAngle2 += 90;
				if (this->position.y > collider->position.y)
				{
					position = Vector2f(position.x - 5, position.y + 5);
					collider->position = Vector2f(collider->position.x + 5, collider->position.y - 5);

				}
				else if (this->position.y < collider->position.y)
				{
					position = Vector2f(position.x - 5, position.y - 5);
					collider->position = Vector2f(collider->position.x + 5, collider->position.y + 5);
				}
			}
			shape.rotate(tempAngle);
			collider->shape.rotate(tempAngle2);
		}
		else if (collider->gameObjectType == Type::bullet)
		{
			deleteNextCycle = true;
			collider->deleteNextCycle = true;
		}
	}	
}

Asteroid::~Asteroid()
{
}
