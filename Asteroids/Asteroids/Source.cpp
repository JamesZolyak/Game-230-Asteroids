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
Text playerScoreText;
Ship* player;
vector<GameObject*> objects;

bool isPlaying = false;
float fireLimiter = -100;
const int startingBigAsteroidCount = 3;
int bigAsteroidCount = startingBigAsteroidCount;
int startingAsteroidLevel;
int asteroidTrackerCount = 0;
const int bigAsteroidSize = 80;
int bigAsteroidSpeedIncrement = 0;
bool ignoreFirstFrame = true;

int score;
enum GameState { mainMenu, inGame, gameOver };
GameState currentState;

Sound shipThruster;
SoundBuffer shipThrusterBuffer;

Sound shipFire;
SoundBuffer shipFireBuffer;

Sound shipExplosion;
SoundBuffer shipExplosionBuffer;

Sound asteroidHit;
SoundBuffer asteroidHitBuffer;

void loadSounds()
{
	shipThrusterBuffer.loadFromFile("ShipThruster.wav");
	shipThruster.setBuffer(shipThrusterBuffer);

	shipFireBuffer.loadFromFile("ShipFire.wav");
	shipFire.setBuffer(shipFireBuffer);

	shipExplosionBuffer.loadFromFile("ShipExplosion.wav");
	shipExplosion.setBuffer(shipExplosionBuffer);

	asteroidHitBuffer.loadFromFile("AsteroidHit.wav");
	asteroidHit.setBuffer(asteroidHitBuffer);
}

void setStartingAsteroidLevel()
{
	startingAsteroidLevel = bigAsteroidCount + (bigAsteroidCount * 2) + (bigAsteroidCount * 2 * 2);
}

void clearObjectList()
{
	for (int j = 0; j < objects.size(); ++j)
	{
		delete objects[j];
		objects[j] = nullptr;
	}
	objects.clear();
	bigAsteroidCount = startingBigAsteroidCount;
	bigAsteroidSpeedIncrement = 0;
}

void resetScore()
{
	score = 0;
}

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
	if (Keyboard::isKeyPressed(Keyboard::Space) && fireLimiter >= 0.25f && currentState == GameState::inGame)
	{
		fireLimiter = 0;
		Bullet* bullet = new Bullet(&window, Vector2f(20, 5), player->shape.getRotation());
		bullet->position = player->shape.getPosition();
		objects.push_back(bullet);
		shipFire.play();
	}
	else if (Keyboard::isKeyPressed(Keyboard::Space) && currentState == GameState::inGame && ignoreFirstFrame)
	{ 
		fireLimiter = 0;
		ignoreFirstFrame = false;
	}
}

void handleCollision()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		for (int j = 0; j < objects.size(); ++j)
		{
			if(objects[i]->shape.getGlobalBounds().intersects(objects[j]->shape.getGlobalBounds()) && objects[i] != objects[j])
				objects[i]->HandleCollision(objects[j], &shipExplosion);
		}
	}
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i]->deleteNextCycle)
		{
			if (objects[i]->gameObjectType == GameObject::Type::asteroid)
			{

				int angle1 = rand() % 361;
				int angle2 = rand() % 361;
				Asteroid* temp1 = new Asteroid(&window, Vector2f(objects[i]->shape.getSize().x / 2, objects[i]->shape.getSize().y / 2), angle1);
				Asteroid* temp2 = new Asteroid(&window, Vector2f(objects[i]->shape.getSize().x / 2, objects[i]->shape.getSize().y / 2), angle2);
				if (objects[i]->gameObjectSize == GameObject::Size::large)
				{
					temp1->gameObjectSize = GameObject::Size::medium;
					temp2->gameObjectSize = GameObject::Size::medium;
					temp1->speed = objects[i]->speed + 50;
					temp2->speed = objects[i]->speed + 50;
					objects.push_back(temp1);
					objects.push_back(temp2);
					score += 10;
				}
				else if (objects[i]->gameObjectSize == GameObject::Size::medium)
				{
					temp1->gameObjectSize = GameObject::Size::tiny;
					temp2->gameObjectSize = GameObject::Size::tiny;
					temp1->speed = objects[i]->speed + 50;
					temp2->speed = objects[i]->speed + 50;
					objects.push_back(temp1);
					objects.push_back(temp2);
					score += 50;
				}
				else if (objects[i]->gameObjectSize == GameObject::Size::tiny)
				{
					score += 100;
				}
				asteroidHit.play();
				asteroidTrackerCount++;
				
				objects.erase(objects.begin() + i);				
			}
			else if (objects[i]->gameObjectType == GameObject::Type::bullet)
			{
				objects.erase(objects.begin() + i);
			}
		}
	}
}

void generateLevel()
{
	asteroidTrackerCount = 0;
	bigAsteroidCount += 1;
	setStartingAsteroidLevel();
	bigAsteroidSpeedIncrement += 20;
	for (int i = 0; i < bigAsteroidCount; ++i)
	{
		int angle = rand() % 361;
		int xPosition = rand() % 801;
		int yPosition = rand() % 601;
		Asteroid* temp = new Asteroid(&window, Vector2f(bigAsteroidSize, bigAsteroidSize), angle);
		temp->position = Vector2f(xPosition, yPosition);
		temp->speed += bigAsteroidSpeedIncrement;
		objects.push_back(temp);
	}
}

void startGame()
{
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		player = new Ship(&window, Vector2f(30, 12.5), &shipThruster);
		player->position = Vector2f(gameWidth/2, gameHeight/2);
		objects.push_back(player);
		generateLevel();
		currentState = GameState::inGame;
		fireLimiter += 2;
	}
}

void handleGameOver()
{
	if (player->lives <= 0)
	{
		currentState = GameState::gameOver;
	}
}

void handleQuit()
{
	if (Keyboard::isKeyPressed(Keyboard::Q))
	{
		window.close();
	}
}

void returnToMainMenu()
{
	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		currentState = GameState::mainMenu;
	}
}

int main()
{
	currentState = GameState::mainMenu;
	ignoreFirstFrame = true;
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	
	Clock clock;
	Font font;
	if (!font.loadFromFile("calibri.ttf"))
		return EXIT_FAILURE;

	loadSounds();

	playerLivesText.setFont(font);
	playerLivesText.setCharacterSize(40);
	playerLivesText.setPosition(660.f, 520.f);
	playerLivesText.setFillColor(Color::White);

	playerScoreText.setFont(font);
	playerScoreText.setCharacterSize(40);
	playerScoreText.setPosition(60.f, 520);
	playerScoreText.setFillColor(Color::White);
	

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
		window.clear();
		if (currentState == GameState::mainMenu)
		{
			Text menuTitle;
			menuTitle.setFont(font);
			menuTitle.setCharacterSize(40);
			menuTitle.setPosition(220, 200);
			menuTitle.setFillColor(Color::White);
			menuTitle.setString("Welcome to Asteroids\n Press Space to play!\nPress Q to quit");
			window.draw(menuTitle);
			handleQuit();
			resetScore();
			clearObjectList();
			startGame();
		}
		else if (currentState == GameState::inGame)
		{
			float deltaTime = clock.restart().asSeconds();
			fireLimiter += deltaTime;

			update_state(deltaTime);
			handleScreenWrap();
			handleFiring();
			handleCollision();
			returnToMainMenu();
			playerScoreText.setString("Score: " + to_string(score));
			playerLivesText.setString("Lives: " + to_string(player->lives));
			window.draw(playerScoreText);
			window.draw(playerLivesText);

			render_frame();
			
			if (startingAsteroidLevel == asteroidTrackerCount)
			{
				generateLevel();
			}
		}
		else if (currentState == GameState::gameOver)
		{
			
			clearObjectList();
			window.draw(playerScoreText);
			returnToMainMenu();

		}

		window.display();
	}

	return 0;
}
