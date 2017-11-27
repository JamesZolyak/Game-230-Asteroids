#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <memory>
#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"

using namespace sf;
using namespace std;

const int gameWidth = 800;
const int gameHeight = 600;

RenderWindow window(VideoMode(gameWidth, gameHeight), "Asteroids!");
Text playerLivesText;
Ship* player;
vector<GameObject*> objects;
bool isPlaying = false;
const int startingBigAsteroidCount = 4;
int bigAsteroidCount;

void update_state(float dt)
{
	for(int i = 0; i < objects.size(); ++i)
	{
		objects[i]->Update(dt);
	}
}

void render_frame()
{
	for(int i = 0; i < objects.size(); ++i)
	{
		objects[i]->Draw();
	}
}

void handleScreenWrap()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i]->position.x < 0)
			objects[i]->position.x = gameWidth;
		if (objects[i]->position.x > gameWidth)
			objects[i]->position.x = 0;
		if (objects[i]->position.y < 0)
			objects[i]->position.y = gameHeight;
		if (objects[i]->position.y > gameHeight)
			objects[i]->position.y = 0;
	}
}

void handleFiring()
{
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		Bullet* bullet = new Bullet(&window, Vector2f(25, 10), player->shape.getRotation());
		bullet->position = player->shape.getPosition();
		objects.push_back(bullet);
	}
}

void handleCollision()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		for (int j = 0; j < objects.size(); ++j)
		{
			if (objects[i] == objects[j])
			{

			}/*ball.getGlobalBounds().intersects(player.paddle.getGlobalBounds()*/
			else if(objects[i]->shape.getGlobalBounds().intersects(objects[j]->shape.getGlobalBounds()))
			{
				objects[j]->HandleCollision(objects[i]);
			}
			objects[i]->Draw();
		}
	}
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	bigAsteroidCount = startingBigAsteroidCount;
	
	Clock clock;
	Font font;
	if (!font.loadFromFile("calibri.ttf"))
		return EXIT_FAILURE;

	player = new Ship(&window, Vector2f(50,25));
	player->position = Vector2f(400, 300);
	objects.push_back(player);
	for (int i = 0; i < bigAsteroidCount; ++i)
	{
		int angle = rand() % 361;
		int xPosition = rand() % 801;
		int yPosition = rand() % 601;
		Asteroid* temp = new Asteroid(&window, Vector2f(100, 100), angle);
		temp->position = Vector2f(xPosition, yPosition);
		objects.push_back(temp);
	}

	playerLivesText.setFont(font);
	playerLivesText.setCharacterSize(40);
	playerLivesText.setPosition(660.f, 520.f);
	playerLivesText.setFillColor(Color::White);
	//playerLivesText.setString("Lives: " + to_string(player->lives));
	playerLivesText.setString(to_string(objects[1]->angle));


	while (window.isOpen())
	{
		if (!isPlaying)
		{
			clock.restart();
			isPlaying = true;
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		float deltaTime = clock.restart().asSeconds();
		window.clear();
		update_state(deltaTime);
		handleScreenWrap();
		handleFiring();
		handleCollision();
		render_frame();
		playerLivesText.setString(to_string(objects[1]->angle));
		window.draw(playerLivesText);
		//window.draw(shape);
		window.display();
	}

	return 0;
}
