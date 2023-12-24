#include <iostream>
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <utility>

class Snake
{
public:
	void run();
	void single_player_mode();
	void onevone_mode();
	void timer_mode();
	bool play_again();
private:
};

enum Object_type { EMPTY, SNAKE, FOOD };

class Base_World;
class Tile;

struct Node
{
 	Tile* data;
	Node* next;
};

struct coordinates
{
	int x, y;
};

class Tile
{
public:
	Tile(int x, int y, Base_World& world);
	virtual void display(sf::RenderWindow& window) {};
	virtual void turn() {};
	virtual Object_type who() { return EMPTY; }
protected:
	coordinates coords;
	Base_World& world;
};

class Empty_Tile : public Tile
{
public:
	virtual void display(sf::RenderWindow& window);
	virtual void turn();
private:
};

class Base_World
{
public:
	Base_World();
	void display_world(sf::RenderWindow& window);
	void run_simulation();
	void simulate_turn();
	std::vector<Tile*>& operator[](int index);
private:
	std::vector<std::vector<Tile*>> world;
};

class snake_guy : public Tile
{
public:
	snake_guy(int x, int y, Base_World& world);
	virtual void display(sf::RenderWindow& window);
	virtual void turn();
	void insertNode(Node* node);
	int getScore() { return score; }
	virtual Object_type who() { return SNAKE; }
private:
	int turns;
	int score;
	Node* head;
	Node* tail;
};

class food : public Tile
{
public:
	food(int x, int y, Base_World& world);
	virtual void display(sf::RenderWindow& window);
	virtual Object_type who() { return FOOD; }
private:
};

Tile::Tile(int x, int y, Base_World& world):world(world)
{
	coords.x = x;
	coords.y = y;
}

Base_World::Base_World()
{
	world.resize(20);
	for (int i = 0; i < 20; i++)
	{
		world[i].resize(20);
	}
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			world[y][x] = new Tile(x, y, *this);
		}
	}

	int randX = rand() % 20;
	int randY = rand() % 20;
	if (world[randY][randX]->who() == EMPTY)
	{
		delete world[randY][randX];
		world[randY][randX] = new snake_guy(randX, randY, *this);
	}

	int food_ready = 6;

	while (food_ready > 0)
	{
		int randX = rand() % 20;
		int randY = rand() % 20;
		if (world[randY][randX]->who() == EMPTY)
		{
			delete world[randY][randX];
			world[randY][randX] = new food(randX, randY, *this);
			food_ready--;
		}
	}
}

snake_guy::snake_guy(int x, int y, Base_World& world):Tile(x, y, world)
{
	score = 0;
	turns = 0;
	head = new Node;
	tail = new Node;
}

food::food(int x, int y, Base_World& world):Tile(x, y, world)
{
	
}

void snake_guy::display(sf::RenderWindow& window)
{
	Node* temp = head;
	while (temp != NULL)
	{
		sf::RectangleShape rect(sf::Vector2f(10, 10));
		rect.setFillColor(sf::Color::Green);
		rect.setPosition(temp->data->coords.x * 20 + 10, temp->data->coords.y * 20 + 10);
		window.draw(rect);
		temp = temp->next;
	}
}

void food::display(sf::RenderWindow& window)
{
	sf::RectangleShape rect(sf::Vector2f(10, 10));
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(coords.x * 20 + 10, coords.y * 20 + 10);
	window.draw(rect);
}

void Base_World::display_world(sf::RenderWindow& window)
{
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			world[y][x]->display(window);
		}
	}
}

void Empty_Tile::display(sf::RenderWindow& window)
{

}

void Base_World::run_simulation()
{

}

void Base_World::simulate_turn()
{
	std::vector<Tile*> foods;
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
 			if (world[y][x]->who() == FOOD)
			{
				foods.push_back(world[y][x]);
			}
		}
	}

	for (int i = 0; i < 100000; i++)
	{
		int index1 = rand() % foods.size();
		int index2 = rand() % foods.size();
		std::swap(foods[index1], foods[index2]);
	}

	for (int i = 0; i < foods.size(); i++)
	{
		foods[i]->turn();
	}

	std::vector<Tile*> snake_guys;
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			if (world[y][x]->who() == SNAKE)
			{
				snake_guys.push_back(world[y][x]);
			}
		}
	}

	for (int i = 0; i < 100000; i++)
	{
		int index1 = rand() % snake_guys.size();
		int index2 = rand() % snake_guys.size();
		std::swap(snake_guys[index1], snake_guys[index2]);
	}

	for (int i = 0; i < snake_guys.size(); i++)
	{
		snake_guys[i]->turn();
	}
}

void Empty_Tile::turn()
{

}

void snake_guy::turn()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		if (coords.y > 0)
		{
			if (world[coords.y - 1][coords.x]->who() == EMPTY)
			{
				std::swap(world[coords.y - 1][coords.x], world[coords.y][coords.x]);
				coords.y--;
			}
			else if (world[coords.y - 1][coords.x]->who() == FOOD)
			{
				insertNode(new Node(new food(coords.x, coords.y, world)));
				delete world[coords.y][coords.x + 1];
				coords.y--;
				score++;
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		if (coords.x < 20 - 1)
		{
			if (world[coords.y][coords.x + 1]->who() == EMPTY)
			{
				std::swap(world[coords.y][coords.x + 1], world[coords.y][coords.x]);
				coords.x++;
			}
			else if (world[coords.y][coords.x + 1]->who() == FOOD)
			{
				insertNode(new Node(new food(coords.x, coords.y, world)));
				delete world[coords.y][coords.x + 1];
				coords.x++;
				score++;
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		if (coords.y < 20 - 1)
		{
			if (world[coords.y + 1][coords.x]->who() == EMPTY)
			{
				std::swap(world[coords.y + 1][coords.x], world[coords.y][coords.x]);
				coords.y++;
			}
			else if (world[coords.y + 1][coords.x]->who() == FOOD)
			{
				insertNode(new Node(new food(coords.x, coords.y, world)));
				delete world[coords.y][coords.x + 1];
				coords.y++;
				score++;
			}
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		if (coords.x > 0)
		{
			if (world[coords.y][coords.x - 1]->who() == EMPTY)
			{
				std::swap(world[coords.y][coords.x - 1], world[coords.y][coords.x]);
				coords.x--;
			}
			else if (world[coords.y][coords.x - 1]->who() == FOOD)
			{
				insertNode(new Node(new food(coords.x, coords.y, world)));
				delete world[coords.y][coords.x + 1];
				coords.x--;
				score++;
			}
		}
	}
}

std::vector<Tile*>& Base_World::operator[](int index)
{
	if (index >= 20 || index < 0)
	{
		std::cout << "Error: index out of bounds" << std::endl;
		exit(1);
	}
	return world[index];
}

void snake_guy::insertNode(Node* node)
{
	Node* newNode = new Node(node->data);

	Node* temp = head;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	temp->next = newNode;
}