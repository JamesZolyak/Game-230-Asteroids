#include "Ship.h"

using namespace sf;

Ship::Ship(RenderWindow* gameWindow, Vector2f shipDimensions, Sound* shipThruster, float radius, Texture* texture)
{
	window = gameWindow;
	speed = BeginningSpeed;
	dimensions = shipDimensions;
	gameObjectType = ship;
	lives = 3;
	shipMovement = *shipThruster;
	circle.setTexture(texture);
	circle.setRadius(radius);
	circle.setOrigin(radius, radius);
	circle.setPosition(defaultPosition);
	circle.setRotation(90);
	deleteNextCycle = false;
}

void Ship::Update(float dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
		circle.rotate(-.08);

	if (Keyboard::isKeyPressed(Keyboard::Right))
		circle.rotate(.08);

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		velocity.y = sin((3.14f / 180)*circle.getRotation()) * dt * speed;
		velocity.x = cos((3.14f / 180)*circle.getRotation())* dt * speed;
		if (shipMovement.getStatus() == Sound::Status::Stopped)
			shipMovement.play();
	}
	if(velocity.x > 0)
		velocity.x -= .00001;
	else if(velocity.x < 0 )
		velocity.x += .00001;
	if (velocity.y > 0)
		velocity.y -= .00001;
	else if (velocity.y < 0)
		velocity.y += .00001;
	//velocity = Normalize();

	position = position - velocity;
	circle.setPosition(position);
}

void Ship::Draw()
{
	window->draw(circle);
}

void Ship::HandleCollision(GameObject* collider, Sound* shipExplosion)
{
	/*if (shape.getGlobalBounds().intersects(collider->shape.getGlobalBounds()))
	{
		if (collider->gameObjectType == Type::asteroid)
		{
			dynamic_cast<Ship*>(collider)->Damage();
			shipExplosion->play();
		}
	}*/
}

void Ship::Damage()
{
	lives--;
	position = defaultPosition;
	circle.setPosition(defaultPosition);
	velocity.x = 0;
	velocity.y = 0;
}

Ship::~Ship()
{
}
