#include "Asteroid.h"



Asteroid::Asteroid(RenderWindow* gameWindow, Vector2f asteroidDimensions, int movementAngle)
{
	window = gameWindow;
	angle = movementAngle;
	dimensions = asteroidDimensions;
	speed = 100;
	shape.setSize(dimensions - sf::Vector2f(3, 3));
	shape.setFillColor(sf::Color::White);
	shape.setOrigin(dimensions / 2.f);
	shape.setRotation(angle);
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

void Asteroid::HandleCollision(GameObject* collider)
{
	if (dynamic_cast<Ship*>(collider) != nullptr)
	{
		dynamic_cast<Ship*>(collider)->lives--;
	}
	else if (dynamic_cast<Bullet*>(collider) != nullptr)
	{

	}
	else if (dynamic_cast<Asteroid*>(collider) != nullptr)
	{
		Vector2f temp = position - dynamic_cast<Asteroid*>(collider)->position;
		float angle = atan2f(temp.y, temp.x);
		int ballAngle = angle;
		
		Vector2f c;
		c.x = cosf(shape.getRotation());
		c.y = sinf(shape.getRotation());
		int tempAngle = atan2f(c.y, -c.x);

		//shape.setRotation(tempAngle);
		//angle = shape.getRotation();

		
		Vector2f d;
		d.x = cosf(dynamic_cast<Asteroid*>(collider)->shape.getRotation());
		d.y = sinf(dynamic_cast<Asteroid*>(collider)->shape.getRotation());
		int tempAngle2 = atan2f(d.y, -d.x);

		//dynamic_cast<Asteroid*>(collider)->shape.setRotation(tempAngle2);
		//dynamic_cast<Asteroid*>(collider)->angle = dynamic_cast<Asteroid*>(collider)->shape.getRotation();
		
		if (this->position.x > dynamic_cast<Asteroid*>(collider)->position.x)
		{
			tempAngle += 90;
			tempAngle2 -= 90;
			if (this->position.y > dynamic_cast<Asteroid*>(collider)->position.y)
			{
				position = Vector2f(position.x + 5, position.y + 5);
				dynamic_cast<Asteroid*>(collider)->position = Vector2f(dynamic_cast<Asteroid*>(collider)->position.x - 5, dynamic_cast<Asteroid*>(collider)->position.y - 5);
			}
			else if (this->position.y < dynamic_cast<Asteroid*>(collider)->position.y)
			{
				position = Vector2f(position.x + 5, position.y - 5);
				dynamic_cast<Asteroid*>(collider)->position = Vector2f(dynamic_cast<Asteroid*>(collider)->position.x - 5, dynamic_cast<Asteroid*>(collider)->position.y + 5);
			}
		}
		else if (this->position.x < dynamic_cast<Asteroid*>(collider)->position.x) 
		{
			tempAngle -= 90;
			tempAngle2 += 90;
			if (this->position.y > dynamic_cast<Asteroid*>(collider)->position.y)
			{
				position = Vector2f(position.x - 5, position.y + 5);
				dynamic_cast<Asteroid*>(collider)->position = Vector2f(dynamic_cast<Asteroid*>(collider)->position.x + 5, dynamic_cast<Asteroid*>(collider)->position.y - 5);

			}
			else if (this->position.y < dynamic_cast<Asteroid*>(collider)->position.y)
			{
				position = Vector2f(position.x - 5, position.y - 5);
				dynamic_cast<Asteroid*>(collider)->position = Vector2f(dynamic_cast<Asteroid*>(collider)->position.x + 5, dynamic_cast<Asteroid*>(collider)->position.y + 5);
			}
		}
		shape.rotate(tempAngle);
		dynamic_cast<Asteroid*>(collider)->shape.rotate(tempAngle2);
		
		
	}
}

Asteroid::~Asteroid()
{
}
