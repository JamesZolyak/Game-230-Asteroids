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
#include "SFML\Effect.hpp"

using namespace sf;
using namespace std;

class StormBlink : public Effect
{
public:
	VertexArray m_points;
	Shader m_shader;
	vector<int> angles;
	StormBlink() :
		Effect("storm + blink")
	{
	}

	bool onLoad()
	{
		// Create the points
		m_points.setPrimitiveType(sf::Points);
		for (int i = 0; i < 300; ++i)
		{
			float x = static_cast<float>(std::rand() % 200);
			float y = static_cast<float>(std::rand() % 200);
			sf::Uint8 r = std::rand() % 255;
			sf::Uint8 g = std::rand() % 255;
			sf::Uint8 b = std::rand() % 255;
			m_points.append(sf::Vertex(sf::Vector2f(x, y), sf::Color(r, g, b)));
			angles.push_back(rand() % 361);
		}

		// Load the shader
		if (!m_shader.loadFromFile("storm.vert", Shader::Type::Vertex ))
			return false;

		return true;
	}

	void onUpdate(float time, float x, float y)
	{
		float radius = 5 + std::cos(time) * 10;
		m_shader.setUniform("storm_position", sf::Vector2f(x * 5, y * 5));
		m_shader.setUniform("storm_inner_radius", radius / 3);
		m_shader.setUniform("storm_total_radius", radius);
		for (int i = 0; i < 300; ++i)
		{
			float tempY = sin(angles[i]) * time * (rand() % 60+32);
			float tempX = cos(angles[i])* time * (rand() % 60 + 32);

			m_points[i].position = m_points[i].position - Vector2f(tempX, tempY);
			//m_points[i].position.x += 1;
		}
		//m_shader.setUniform("blink_alpha", 0.5f + std::cos(time * 3) * 0.25f);
	}

	void startPoints(Vector2f positionStart)
	{
		for (int i = 0; i < 300; ++i)
		{
			m_points[i].position = positionStart;
		}
	}

	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.shader = &m_shader;
		target.draw(m_points, states);
	}
	
};
const int gameWidth = 1000;
const int gameHeight = 1000;
const float BUCKET_WIDTH = 100;
const float BUCKET_HEIGHT = 100;
const int COLUMNS = 10;
const int ROWS = 10;
vector<GameObject*> grid[COLUMNS][ROWS];

RenderWindow window(VideoMode(gameWidth, gameHeight), "Asteroids!");
Text playerLivesText;
Text playerScoreText;
Ship* player;
vector<GameObject*> objects;
vector<GameObject*> buckets[5];

bool isPlaying = false;
float fireLimiter = -100;
const int startingBigAsteroidCount = 3;
int bigAsteroidCount = startingBigAsteroidCount;
int startingAsteroidLevel;
int asteroidTrackerCount = 0;
const int bigAsteroidSize = 80;
const int bigAsteroidRadius = 60;
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

Sound newLevel;
SoundBuffer newLevelBuffer;

Texture shipTexture;
Texture asteroidTexture;

Clock gameClock;
StormBlink storm1;
StormBlink storm2;
StormBlink storm3;
StormBlink storm4;
StormBlink storm5;
float drawShaderCount1 = 0;
float drawShaderCount2 = 0;
float drawShaderCount3 = 0;
float drawShaderCount4 = 0;
float drawShaderCount5 = 0;
Vector2f explosionPosition1;
Vector2f explosionPosition2;
Vector2f explosionPosition3;
Vector2f explosionPosition4;
Vector2f explosionPosition5;
const float shaderTimeLength = 1.5f;

void addGameObject(GameObject* obj)
{
	objects.push_back(obj);
	int i = obj->getRenderBucket();
	buckets[i].push_back(obj);
}

float length(Vector2f v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

void loadSoundsAndTextures()
{
	shipThrusterBuffer.loadFromFile("ShipThruster.wav");
	shipThruster.setBuffer(shipThrusterBuffer);

	shipFireBuffer.loadFromFile("ShipFire.wav");
	shipFire.setBuffer(shipFireBuffer);

	shipExplosionBuffer.loadFromFile("ShipExplosion.wav");
	shipExplosion.setBuffer(shipExplosionBuffer);

	asteroidHitBuffer.loadFromFile("AsteroidHit.wav");
	asteroidHit.setBuffer(asteroidHitBuffer);

	newLevelBuffer.loadFromFile("NewLevel.wav");
	newLevel.setBuffer(newLevelBuffer);

	shipTexture.loadFromFile("nightraiderfixed.png");
	asteroidTexture.loadFromFile("AsteroidHuge.png");

	storm1.load();
	storm2.load();
	storm3.load();
	storm4.load();
	storm5.load();
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
	ignoreFirstFrame = true;
}

void resetScore()
{
	score = 0;
}

Vector2i getBucket(Vector2f pos)
{
	int col = int(pos.x / BUCKET_WIDTH);
	if (col < 0)
		col = 0;
	else if (col >= COLUMNS)
		col = COLUMNS - 1;

	int row = int(pos.y / BUCKET_HEIGHT);
	if (row < 0)
		row = 0;
	else if (row >= ROWS)
		row = ROWS - 1;

	return Vector2i(col, row);
}

void bucket_add(Vector2i b, GameObject* obj)
{
	vector<GameObject*> & v = grid[b.x][ b.y];

	v.push_back(obj);
}

void bucket_remove(Vector2i b, GameObject* obj)
{
	vector<GameObject*> & v = grid[b.x][ b.y];
	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i] == obj)
		{
			v.erase(v.begin() + i);
			break;
		}
	}
}

void detect_collisions(GameObject* obj, Vector2i b)
{
	int left = max(b.x - 1, 0);
	int right = min(b.x + 1, COLUMNS - 1);
	int top = max(b.y - 1, 0);
	int bot = min(b.y + 1, ROWS - 1);

	for (int bx = left; bx <= right; ++bx)
	{
		for (int by = top; by <= bot; ++by)
		{
			vector<GameObject*> & v = grid[bx][ by];
			for (GameObject* o : v)
			{
				if (o != obj)
				{
					float d = length(obj->circle.getPosition() - o->circle.getPosition());
					float sum = obj->circle.getRadius() + o->circle.getRadius();

					if (d < sum)
					{
						obj->HandleCollision(o, &shipExplosion);
						if (obj->gameObjectType != o->gameObjectType && obj->gameObjectType == GameObject::Type::asteroid)
						{
							if (drawShaderCount1 <= 0)
							{
								drawShaderCount1 = shaderTimeLength;
								storm1.startPoints(obj->position);
								explosionPosition1 = obj->position;
							}
							else if (drawShaderCount2 <= 0)
							{
								drawShaderCount2 = shaderTimeLength;
								storm2.startPoints(obj->position);
								explosionPosition2 = obj->position;
							}
							else if (drawShaderCount3 <= 0)
							{
								drawShaderCount3 = shaderTimeLength;
								storm3.startPoints(obj->position);
								explosionPosition3 = obj->position;
							}
							else if (drawShaderCount4 <= 0)
							{
								drawShaderCount4 = shaderTimeLength;
								storm4.startPoints(obj->position);
								explosionPosition4 = obj->position;
							}
							else if (drawShaderCount5 <= 0)
							{
								drawShaderCount5 = shaderTimeLength;
								storm5.startPoints(obj->position);
								explosionPosition5 = obj->position;
							}
						}
					}
				}
			}
		}
	}
}



void update_state(float dt)
{
	for(int i = 0; i < objects.size(); ++i)
	{
		GameObject * obj = objects[i];
		Vector2i curBucket = getBucket(obj->getCenter());
		obj->Update(dt);
		Vector2i newBucket = getBucket(obj->getCenter());
		if (curBucket != newBucket)
		{
			bucket_remove(curBucket, obj);
			bucket_add(newBucket, obj);
		}
		detect_collisions(obj, newBucket);

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
		Bullet* bullet = new Bullet(&window, Vector2f(20, 5), 5, player->circle.getRotation());
		bullet->position = player->circle.getPosition();
		addGameObject(bullet);
		shipFire.play();
	}
	else if (Keyboard::isKeyPressed(Keyboard::Space) && currentState == GameState::inGame && ignoreFirstFrame)
	{ 
		fireLimiter = 0;
		ignoreFirstFrame = false;
	}
}

void handleDeleteCycle()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i]->deleteNextCycle)
		{
			if (objects[i]->gameObjectType == GameObject::Type::asteroid)
			{

				int tempX = objects[i]->position.x;
				int tempY = objects[i]->position.y;
				int angle1 = rand() % 361;
				int angle2 = rand() % 361;
				Vector2f tempPosition1 = Vector2f(tempX + 75, tempY + 75);
				Vector2f tempPosition2 = Vector2f(tempX - 75, tempY - 75);
				Asteroid* temp1 = new Asteroid(&window, Vector2f(objects[i]->shape.getSize().x / 2, objects[i]->shape.getSize().y / 2), angle1, objects[i]->circle.getRadius() / 2, &asteroidTexture);
				Asteroid* temp2 = new Asteroid(&window, Vector2f(objects[i]->shape.getSize().x / 2, objects[i]->shape.getSize().y / 2), angle2, objects[i]->circle.getRadius() / 2, &asteroidTexture);
				temp1->position = tempPosition1;
				temp2->position = tempPosition2;
				if (objects[i]->gameObjectSize == GameObject::Size::large)
				{
					temp1->gameObjectSize = GameObject::Size::medium;
					temp2->gameObjectSize = GameObject::Size::medium;
					temp1->speed = objects[i]->speed + 50;
					temp2->speed = objects[i]->speed + 50;
					addGameObject(temp1);
					addGameObject(temp2);
					score += 10;
				}
				else if (objects[i]->gameObjectSize == GameObject::Size::medium)
				{
					temp1->gameObjectSize = GameObject::Size::tiny;
					temp2->gameObjectSize = GameObject::Size::tiny;
					temp1->speed = objects[i]->speed + 50;
					temp2->speed = objects[i]->speed + 50;
					addGameObject(temp1);
					addGameObject(temp2);
					score += 50;
				}

				else if (objects[i]->gameObjectSize == GameObject::Size::tiny)
				{
					score += 100;
				}
				asteroidHit.play();
				asteroidTrackerCount++;
				
				delete objects[i];
				objects.erase(objects.begin() + i);
			}
			else if (objects[i]->gameObjectType == GameObject::Type::bullet)
			{
				delete objects[i];
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
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i]->gameObjectType == GameObject::Type::bullet)
		{
			delete objects[i];
			objects[i] = nullptr;
			objects.erase(objects.begin() + i);
		}
	}
	for (int i = 0; i < bigAsteroidCount; ++i)
	{
		int angle = rand() % 361;
		int xPosition = rand() % (gameWidth + 1);
		int yPosition = rand() % (gameHeight + 1);
		Asteroid* temp = new Asteroid(&window, Vector2f(bigAsteroidSize, bigAsteroidSize), angle, bigAsteroidRadius, &asteroidTexture);
		temp->position = Vector2f(xPosition, yPosition);
		temp->speed += bigAsteroidSpeedIncrement;
		addGameObject(temp);
	}
}

void startGame()
{
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		player = new Ship(&window, Vector2f(30, 12.5), &shipThruster, 20, &shipTexture);
		player->position = Vector2f(gameWidth/2, gameHeight/2);
		addGameObject(player);
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
	
	Font font;
	if (!font.loadFromFile("calibri.ttf"))
		return EXIT_FAILURE;

	loadSoundsAndTextures();

	playerLivesText.setFont(font);
	playerLivesText.setCharacterSize(40);
	playerLivesText.setPosition(gameWidth-160, gameHeight-80);
	playerLivesText.setFillColor(Color::White);

	playerScoreText.setFont(font);
	playerScoreText.setCharacterSize(40);
	playerScoreText.setPosition(0+60, gameHeight - 80);
	playerScoreText.setFillColor(Color::White);
	

	while (window.isOpen())
	{
		if (!isPlaying)
		{
			gameClock.restart();
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
			float deltaTime = gameClock.restart().asSeconds();
			fireLimiter += deltaTime;

			update_state(deltaTime);
			handleDeleteCycle();
			handleScreenWrap();
			handleFiring();
			returnToMainMenu();
			handleGameOver();
			playerScoreText.setString("Score: " + to_string(score));
			playerLivesText.setString("Lives: " + to_string(player->lives));
			window.draw(playerScoreText);
			window.draw(playerLivesText);
			if (drawShaderCount1 > 0)
			{

				storm1.update(deltaTime, explosionPosition1.x, explosionPosition1.y);
				drawShaderCount1 -= deltaTime;
				window.draw(storm1);
			}
			if (drawShaderCount2 > 0)
			{

				storm2.update(deltaTime, explosionPosition2.x, explosionPosition2.y);
				drawShaderCount2 -= deltaTime;
				window.draw(storm2);
			}
			if (drawShaderCount3 > 0)
			{

				storm3.update(deltaTime, explosionPosition3.x, explosionPosition3.y);
				drawShaderCount3 -= deltaTime;
				window.draw(storm3);
			}
			if (drawShaderCount4 > 0)
			{

				storm4.update(deltaTime, explosionPosition4.x, explosionPosition4.y);
				drawShaderCount4 -= deltaTime;
				window.draw(storm4);
			}
			if (drawShaderCount5 > 0)
			{

				storm5.update(deltaTime, explosionPosition5.x, explosionPosition5.y);
				drawShaderCount5 -= deltaTime;
				window.draw(storm5);
			}

			render_frame();
			
			if (startingAsteroidLevel == asteroidTrackerCount)
			{
				generateLevel();
				newLevel.play();
			}

		}
		else if (currentState == GameState::gameOver)
		{
			handleQuit();
			Text gameOverText;
			gameOverText.setFont(font);
			gameOverText.setCharacterSize(40);
			gameOverText.setPosition(220, 200);
			gameOverText.setFillColor(Color::White);
			gameOverText.setString("You Lose!\n Press Escape to return to the main Menu!\nPress Q to quit");
			clearObjectList();
			window.draw(gameOverText);
			window.draw(playerScoreText);
			returnToMainMenu();

		}

		window.display();
	}

	return 0;
}
