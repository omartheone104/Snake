#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

const int BOARD_HEIGHT = 20;
const int BOARD_WIDTH = 20;
const int SQUARE_HEIGHT = 40;
const int SQUARE_WIDTH = 40;

enum Direction { NONE, UP, RIGHT, DOWN, LEFT, W, D, S, A };

struct coordinates
{
	int x;
	int	y;
};

struct node
{
	coordinates coords{};
	sf::RectangleShape shape;
};

class World
{
public:
	World();
	void display_world(sf::RenderWindow& window);
private:
	std::vector<sf::RectangleShape> world;
};

class snake_guy;
class Food;

class Food
{
public:
	Food();
	void display(sf::RenderWindow& window);
	int getXPosition() { return food_obj.begin()->coords.x; }
	int getYPosition() { return food_obj.begin()->coords.y; }
	node getdata(int num);
	void generate_food(snake_guy guy);
	int getsize() { return food_obj.size(); }
	bool notunqiue(snake_guy snake, coordinates coord);
	void eat(snake_guy snake);
private:
	std::list<node> food_obj;
};

class snake_guy
{
public:
	snake_guy();
	snake_guy(int x, int y);
	void display(sf::RenderWindow& window);
	Direction keypressed();
	Direction keypressed2();
	void move(Food food);
	void move2(Food food);
	void game_over(sf::RenderWindow& window);
	void hits(snake_guy snake);
	int getXPosition() { return snake_obj.begin()->coords.x; }
	int getYPosition() { return snake_obj.begin()->coords.y; }
	int getScore() { return score; }
	bool isDead() { return dead; }
	void setDead(bool dead) { this->dead = dead; }
	bool getSpeed() { return speed; }
private:
	Direction desired_direction;
	Direction current_direction;
	std::list<node> snake_obj;
	bool dead;
	int score;
	bool speed;
};

World::World()
{
	for (int i = 0; i < BOARD_HEIGHT; i++)
	{
		for (int j = 0; j < BOARD_WIDTH; j++)
		{
			sf::RectangleShape square(sf::Vector2f(SQUARE_WIDTH, SQUARE_HEIGHT));
			square.setPosition((float)j * SQUARE_WIDTH, (float)i * SQUARE_HEIGHT);
			if (i % 2 == j % 2)
			{
				sf::Color color(0, 102, 17);
				square.setFillColor(color);
			}
			else
			{
				sf::Color color(100, 255, 0);
				square.setFillColor(color);
			}
			world.push_back(square);
		}
	}
}

snake_guy::snake_guy()
{
	srand(time(nullptr));
	score = 0;
	speed = false;
	desired_direction = NONE;
	current_direction = NONE;
	dead = false;
	coordinates coords;
	coords.x = rand() % BOARD_WIDTH / 2 + 5;
	coords.y = rand() % BOARD_HEIGHT / 2 + 5;
	node head;
	head.coords = coords;
	sf::RectangleShape rect(sf::Vector2f(SQUARE_WIDTH, SQUARE_HEIGHT));
	rect.setPosition((float)coords.x * SQUARE_WIDTH, (float)coords.y * SQUARE_HEIGHT);
	rect.setFillColor(sf::Color::Blue);
	head.shape = rect;
	snake_obj.push_front(head);
}

snake_guy::snake_guy(int x, int y)
{
	score = 0;
	desired_direction = NONE;
	current_direction = NONE;
	dead = false;
	speed = false;
	coordinates coords;
	coords.x = x;
	coords.y = y;
	node head;
	head.coords = coords;
	sf::RectangleShape rect(sf::Vector2f(SQUARE_WIDTH, SQUARE_HEIGHT));
	rect.setPosition((float)x * SQUARE_WIDTH, (float)y * SQUARE_HEIGHT);
	rect.setFillColor(sf::Color::Black);
	head.shape = rect;
	snake_obj.push_front(head);
}

Food::Food()
{
	srand(time(nullptr));
	coordinates coords;
	coords.x = rand() % BOARD_WIDTH - 1;
	coords.y = rand() % BOARD_HEIGHT - 1;
	node head;
	head.coords = coords;
	sf::RectangleShape rect(sf::Vector2f(SQUARE_WIDTH, SQUARE_HEIGHT));
	rect.setPosition((float)coords.x * SQUARE_WIDTH, (float)coords.y * SQUARE_HEIGHT);
	rect.setFillColor(sf::Color::Red);
	head.shape = rect;
}

void World::display_world(sf::RenderWindow& window)
{
	for (int i = 0; i < world.size(); i++)
	{
		window.draw(world[i]);
	}
}

void snake_guy::display(sf::RenderWindow& window)
{
	for (std::list<node>::iterator it = snake_obj.begin(); it != snake_obj.end(); it++)
	{
		window.draw(it->shape);
	}
}

void Food::display(sf::RenderWindow& window)
{
	for (std::list<node>::iterator it = food_obj.begin(); it != food_obj.end(); it++)
	{
		window.draw(it->shape);
	}
}

void snake_guy::game_over(sf::RenderWindow& window)
{
	if (dead)
	{
		sf::Texture texture;
		if (!texture.loadFromFile("315188481229211.png"))
		{
			std::cout << "Error: could not load texture file" << std::endl;
			exit(1);
		}
		texture.setSmooth(true);
		sf::Sprite dead;
		dead.setTexture(texture);
		dead.setScale(0.15f, 0.15f);
		dead.setPosition(125, 300);
		window.draw(dead);
	}
}

Direction snake_guy::keypressed()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		desired_direction = UP;
		if(current_direction != DOWN)
			current_direction = UP;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		desired_direction = RIGHT;
		if (current_direction != LEFT)
			current_direction = RIGHT;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		desired_direction = DOWN;
		if (current_direction != UP)
			current_direction = DOWN;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		desired_direction = LEFT;
		if (current_direction != RIGHT)
			current_direction = LEFT;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
	{
		speed = true;
	}

	return current_direction;
}

Direction snake_guy::keypressed2()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		desired_direction = W;
		if (current_direction != S)
			current_direction = W;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		desired_direction = D;
		if (current_direction != A)
			current_direction = D;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		desired_direction = S;
		if (current_direction != W)
			current_direction = S;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		desired_direction = A;
		if (current_direction != D)
			current_direction = A;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
	{
		speed = true;
	}
	return current_direction;
}

void snake_guy::move2(Food food)
{
	node move_head2;
	move_head2.coords.x = snake_obj.begin()->coords.x;
	move_head2.coords.y = snake_obj.begin()->coords.y;
	sf::RectangleShape rect(sf::Vector2f(SQUARE_WIDTH, SQUARE_HEIGHT));
	rect.setFillColor(sf::Color::Black);
	bool eaten = false;
	speed = false;

	if (keypressed2() == W && !dead)
	{
		rect.setPosition((float)move_head2.coords.x * SQUARE_WIDTH, (float)--move_head2.coords.y * SQUARE_HEIGHT);
		move_head2.shape = rect;
		snake_obj.push_front(move_head2);
		snake_obj.pop_back();
		for (int i = 0; i < food.getsize(); ++i)
		{
			if (move_head2.coords.x == food.getdata(i).coords.x && move_head2.coords.y == food.getdata(i).coords.y)
			{
				eaten = true;
				score++;
				move_head2.shape = rect;
				snake_obj.push_front(move_head2);
			}
			if (!eaten)
			{
				for (auto i = snake_obj.begin(); i != snake_obj.end(); ++i)
				{
					if (i->coords.x == snake_obj.begin()->coords.x && i->coords.y == snake_obj.begin()->coords.y && i != snake_obj.begin())
					{
						dead = true;
					}
				}
			}
		}
		if (move_head2.coords.y < 0)
		{
			dead = true;
		}
	}
	if (keypressed2() == D && !dead)
	{
		rect.setPosition((float)++move_head2.coords.x * SQUARE_WIDTH, (float)move_head2.coords.y * SQUARE_HEIGHT);
		move_head2.shape = rect;
		snake_obj.push_front(move_head2);
		snake_obj.pop_back();
		for (int i = 0; i < food.getsize(); ++i)
		{
			if (move_head2.coords.x == food.getdata(i).coords.x && move_head2.coords.y == food.getdata(i).coords.y)
			{
				eaten = true;
				score++;
				move_head2.shape = rect;
				snake_obj.push_front(move_head2);
			}
			if (!eaten)
			{
				for (auto i = snake_obj.begin(); i != snake_obj.end(); ++i)
				{
					if (i->coords.x == snake_obj.begin()->coords.x && i->coords.y == snake_obj.begin()->coords.y && i != snake_obj.begin())
					{
						dead = true;
					}
				}
			}
		}
		if (move_head2.coords.x > BOARD_WIDTH - 1)
		{
			dead = true;
		}
	}
	if (keypressed2() == S && !dead)
	{
		rect.setPosition((float)move_head2.coords.x * SQUARE_WIDTH, (float)++move_head2.coords.y * SQUARE_HEIGHT);
		move_head2.shape = rect;
		snake_obj.push_front(move_head2);
		snake_obj.pop_back();
		for (int i = 0; i < food.getsize(); ++i)
		{
			if (move_head2.coords.x == food.getdata(i).coords.x && move_head2.coords.y == food.getdata(i).coords.y)
			{
				eaten = true;
				score++;
				move_head2.shape = rect;
				snake_obj.push_front(move_head2);
			}
			if (!eaten)
			{
				for (auto i = snake_obj.begin(); i != snake_obj.end(); ++i)
				{
					if (i->coords.x == snake_obj.begin()->coords.x && i->coords.y == snake_obj.begin()->coords.y && i != snake_obj.begin())
					{
						dead = true;
					}
				}
			}
		}
		if (move_head2.coords.y > BOARD_HEIGHT - 1)
		{
			dead = true;
		}
	}
	if (keypressed2() == A && !dead)
	{
		rect.setPosition((float)--move_head2.coords.x * SQUARE_WIDTH, (float)move_head2.coords.y * SQUARE_HEIGHT);
		move_head2.shape = rect;
		snake_obj.push_front(move_head2);
		snake_obj.pop_back();
		for (int i = 0; i < food.getsize(); ++i)
		{
			if (move_head2.coords.x == food.getdata(i).coords.x && move_head2.coords.y == food.getdata(i).coords.y)
			{
				eaten = true;
				score++;
				move_head2.shape = rect;
				snake_obj.push_front(move_head2);
			}
			if (!eaten)
			{
				for (auto i = snake_obj.begin(); i != snake_obj.end(); ++i)
				{
					if (i->coords.x == snake_obj.begin()->coords.x && i->coords.y == snake_obj.begin()->coords.y && i != snake_obj.begin())
					{
						dead = true;
					}
				}
			}
		}
		if (move_head2.coords.x < 0)
		{
			dead = true;
		}
	}
}

void snake_guy::move(Food food)
{
	node move_head;
	move_head.coords.x = snake_obj.begin()->coords.x;
	move_head.coords.y = snake_obj.begin()->coords.y;
	sf::RectangleShape rect(sf::Vector2f(SQUARE_WIDTH, SQUARE_HEIGHT));
	rect.setFillColor(sf::Color::Blue);
	bool eaten = false;
	speed = false;

	if (keypressed() == UP && !dead)
	{
		rect.setPosition((float)move_head.coords.x * SQUARE_WIDTH, (float)--move_head.coords.y * SQUARE_HEIGHT);
		move_head.shape = rect;
		snake_obj.push_front(move_head);
		snake_obj.pop_back();
		for (int i = 0; i < food.getsize(); ++i)
		{
			if (move_head.coords.x == food.getdata(i).coords.x && move_head.coords.y == food.getdata(i).coords.y)
			{
				score++;
				move_head.shape = rect;
				snake_obj.push_front(move_head);
				eaten = true;
			}
			if(!eaten)
			{
				for (auto j = snake_obj.begin(); j != snake_obj.end(); ++j)
				{
					if (j->coords.x == snake_obj.begin()->coords.x && j->coords.y == snake_obj.begin()->coords.y && j != snake_obj.begin())
					{
						dead = true;
					}
				}
			}
		}
		if (move_head.coords.y < 0)
		{
			dead = true;
		}
	}
	if (keypressed() == RIGHT && !dead)
	{
		rect.setPosition((float)++move_head.coords.x * SQUARE_WIDTH, (float)move_head.coords.y * SQUARE_HEIGHT);
		move_head.shape = rect;
		snake_obj.push_front(move_head);
		snake_obj.pop_back();
		for (int i = 0; i < food.getsize(); ++i)
		{
			if (move_head.coords.x == food.getdata(i).coords.x && move_head.coords.y == food.getdata(i).coords.y)
			{
				eaten = true;
				score++;
				move_head.shape = rect;
				snake_obj.push_front(move_head);
				
			}
			if(!eaten)
			{
				for (auto j = snake_obj.begin(); j != snake_obj.end(); ++j)
				{
					if (j->coords.x == snake_obj.begin()->coords.x && j->coords.y == snake_obj.begin()->coords.y && j != snake_obj.begin())
					{
						//std::cout << "dead" << std::endl;
						dead = true;
					}
				}
			}
		}
		if (move_head.coords.x > BOARD_WIDTH - 1)
		{
			dead = true;
		}
	}
	if (keypressed() == DOWN && !dead)
	{
		rect.setPosition((float)move_head.coords.x * SQUARE_WIDTH, (float)++move_head.coords.y * SQUARE_HEIGHT);
		move_head.shape = rect;
		snake_obj.push_front(move_head);
		snake_obj.pop_back();
		for (int i = 0; i < food.getsize(); ++i)
		{
			if (move_head.coords.x == food.getdata(i).coords.x && move_head.coords.y == food.getdata(i).coords.y)
			{
				eaten = true;
				score++;
				move_head.shape = rect;
				snake_obj.push_front(move_head);
			}
			if(!eaten)
			{
				for (auto j = snake_obj.begin(); j != snake_obj.end(); ++j)
				{
					if (j->coords.x == snake_obj.begin()->coords.x && j->coords.y == snake_obj.begin()->coords.y && j != snake_obj.begin())
					{
						dead = true;
					}
				}
			}
		}
		if (move_head.coords.y > BOARD_HEIGHT - 1)
		{
			dead = true;
		}
	}
	if (keypressed() == LEFT && !dead)
	{
		rect.setPosition((float)--move_head.coords.x * SQUARE_WIDTH, (float)move_head.coords.y * SQUARE_HEIGHT);
		move_head.shape = rect;
		snake_obj.push_front(move_head);
		snake_obj.pop_back();
		for (int i = 0; i < food.getsize(); ++i)
		{
			if (move_head.coords.x == food.getdata(i).coords.x && move_head.coords.y == food.getdata(i).coords.y)
			{
				eaten = true;
				score++;
				move_head.shape = rect;
				snake_obj.push_front(move_head);
			}
			if(!eaten)
			{
				for (auto j = snake_obj.begin(); j != snake_obj.end(); ++j)
				{
					if (j->coords.x == snake_obj.begin()->coords.x && j->coords.y == snake_obj.begin()->coords.y && j != snake_obj.begin())
					{
						dead = true;
					}
				}
			}
		}
		if (move_head.coords.x < 0)
		{
			dead = true;
		}
	}
}

void Food::generate_food(snake_guy guy)
{	
	node food;
	sf::RectangleShape rect(sf::Vector2f(SQUARE_WIDTH, SQUARE_HEIGHT));
	rect.setFillColor(sf::Color::Red);
	coordinates coords;
	food.coords.x = rand() % BOARD_WIDTH - 1;
	food.coords.y = rand() % BOARD_HEIGHT - 1;
	while (notunqiue(guy, food.coords))
	{
		food.coords.x = rand() % BOARD_WIDTH - 1;
		food.coords.y = rand() % BOARD_HEIGHT - 1;
	}
	rect.setPosition((float)food.coords.x * SQUARE_WIDTH, (float)food.coords.y * SQUARE_HEIGHT);
	food.shape = rect;
	food_obj.push_front(food);
}

node Food::getdata(int num)
{
	int k = 0;
	for (auto i = food_obj.begin(); i != food_obj.end(); ++i)
	{
		if (k == num)
		{
			return *i;
		}
		k++;
	}
}

void snake_guy::hits(snake_guy snake)
{
	for (auto i = snake_obj.begin(); i != snake_obj.end(); ++i)
	{
		if (i->coords.x == snake.snake_obj.begin()->coords.x && i->coords.y == snake.snake_obj.begin()->coords.y)
		{
			dead = true;
		}
	}
}

bool Food::notunqiue(snake_guy snake, coordinates coord)
{
	for (auto i = food_obj.begin(); i != food_obj.end(); ++i)
	{
		if (i->coords.x == snake.getXPosition() && i->coords.y == snake.getYPosition())
		{
			return true;
		}
		if (i->coords.x == coord.x && i->coords.y == coord.y)
		{
			return true;
		}
	}
	return false;
}

void Food::eat(snake_guy snake)
{
	for (auto i = food_obj.begin(); i != food_obj.end(); ++i)
	{
		//std::cout << "(" << i->coords.x << " " << i->coords.y << ")" << " " << snake.getXPosition() << " " << snake.getYPosition() << std::endl;
		if (i->coords.x == snake.getXPosition() && i->coords.y == snake.getYPosition())
		{
			//std::cout << food_obj.size() << std::endl;
			i = food_obj.erase(i);
			//std::cout << food_obj.size() << std::endl;
		}
	}
	//std::cout << "-------" << std::endl;
}