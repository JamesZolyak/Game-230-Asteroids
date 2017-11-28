#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class GameObject
{
public:
	enum Type { bullet, ship, asteroid };
	enum Size { large, medium, tiny };
	Type gameObjectType;
	Size gameObjectSize;
	RenderWindow* window;
	RectangleShape shape;
	Vector2f position;
	Vector2f velocity;
	Vector2f dimensions;
	int angle;
	int speed;
	bool deleteNextCycle = false;

	GameObject();
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual void HandleCollision(GameObject* collider, Sound* shipExplosion) = 0;
	virtual int getRenderBucket() = 0;
	virtual Vector2f getCenter() = 0;
	float Length();
	Vector2f Normalize();
	~GameObject();
};

