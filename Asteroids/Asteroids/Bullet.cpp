#include "Bullet.h"



Bullet::Bullet(RenderWindow* gameWindow, Vector2f bulletDimensions, int bulletAngle)
{
	window = gameWindow;
	speed = 400;
	angle = bulletAngle;
	shape.rotate(angle);
	dimensions = bulletDimensions;
	shape.setSize(dimensions - sf::Vector2f(3, 3));
	shape.setFillColor(sf::Color::Red);
	shape.setOrigin(dimensions / 2.f);
	//shape.setRotation(90);
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

void Bullet::HandleCollision(GameObject* collider)
{

}

Bullet::~Bullet()
{
}
