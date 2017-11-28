#include "Bullet.h"



Bullet::Bullet(RenderWindow* gameWindow, Vector2f bulletDimensions, int bulletAngle)
{
	window = gameWindow;
	speed = 400;
	angle = bulletAngle;
	shape.rotate(angle);
	dimensions = bulletDimensions;
	gameObjectType = bullet;
	shape.setSize(dimensions - sf::Vector2f(3, 3));
	shape.setFillColor(sf::Color::Red);
	shape.setOrigin(dimensions / 2.f);
	//shape.setRotation(90);
	deleteNextCycle = false;
}

void Bullet::Update(float dt)
{
	velocity.y = sin((3.14f / 180)*angle) * dt * speed;
	velocity.x = cos((3.14f / 180)*angle)* dt * speed;

	position = position - velocity;
	shape.setPosition(position);
}

void Bullet::Draw()
{
	window->draw(shape);
}

void Bullet::HandleCollision(GameObject* collider, Sound* shipExplosion)
{
	/*if (shape.getGlobalBounds().intersects(collider->shape.getGlobalBounds()))
	{
		if (collider->gameObjectType == Type::asteroid)
		{
			deleteNextCycle = true;
			collider->deleteNextCycle = true;
		}
	}*/
}

Bullet::~Bullet()
{
}
