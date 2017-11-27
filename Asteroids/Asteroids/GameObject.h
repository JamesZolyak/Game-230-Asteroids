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

	RenderWindow* window;
	RectangleShape shape;
	Vector2f position;
	Vector2f velocity;
	Vector2f dimensions;
	int angle;
	int speed;
	const float gravity = 9.8f;

	GameObject();
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual void HandleCollision(GameObject* collider) = 0;
	float Length();
	Vector2f Normalize();
	~GameObject();
};

