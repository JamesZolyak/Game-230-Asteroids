#include "Asteroid.h"



Asteroid::Asteroid(RenderWindow* gameWindow, Vector2f asteroidDimensions, int movementAngle, float radius, Texture* texture)
{
	window = gameWindow;
	angle = movementAngle;
	dimensions = asteroidDimensions;
	speed = 20;
	gameObjectType = asteroid;
	gameObjectSize = large;
	//shape.setSize(dimensions - sf::Vector2f(3, 3));
	//shape.setFillColor(sf::Color::White);
	//shape.setOrigin(dimensions / 2.f);
	//shape.setRotation(angle);
	circle.setRadius(radius);
	circle.setTexture(texture);
	//circle.setTextureRect(IntRect(circle.getGlobalBounds().left, circle.getGlobalBounds().top, radius * 4, radius * 4));
	//circle.setFillColor(sf::Color::White);
	/*circle.setOutlineThickness(2);
	circle.setOutlineColor(Color::Cyan);*/
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
	//shape.setPosition(position);
}

void Asteroid::Draw()
{
	//window->draw(shape);
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
