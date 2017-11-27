#include "Ship.h"

using namespace sf;

Ship::Ship(RenderWindow* gameWindow, Vector2f shipDimensions)
{
	window = gameWindow;
	speed = BeginningSpeed;
	dimensions = shipDimensions;
	gameObjectType = ship;
	lives = 3;
	shape.setSize(dimensions - sf::Vector2f(3, 3));
	shape.setOutlineThickness(3);
	shape.setOutlineColor(sf::Color::Black);
	shape.setFillColor(sf::Color(100, 100, 200));
	shape.setOrigin(dimensions / 2.f);
	shape.setPosition(defaultPosition);
	shape.setRotation(90);
	deleteNextCycle = false;
}

void Ship::Update(float dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
		shape.rotate(-.05);

	if (Keyboard::isKeyPressed(Keyboard::Right))
		shape.rotate(.05);

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		velocity.y = sin((3.14f / 180)*shape.getRotation()) * dt * speed;
		velocity.x = cos((3.14f / 180)*shape.getRotation())* dt * speed;
	}
	if(velocity.x > 0)
		velocity.x -= .00001;
	if(velocity.y > 0)
		velocity.y -=  .00001;
	//velocity = Normalize();
	position = position - velocity;
	shape.setPosition(position);
}

void Ship::Draw()
{
	window->draw(shape);
}

void Ship::HandleCollision(GameObject* collider)
{

}

void Ship::Damage()
{
	lives--;
	position = defaultPosition;
	shape.setPosition(defaultPosition);
}

Ship::~Ship()
{
}
