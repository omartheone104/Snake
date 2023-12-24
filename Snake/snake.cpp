#include "snakegame.h"

int main()
{
	Snake snake;
	do
	{
		snake.run();
	} while (snake.play_again());
	return 0;
}

void Snake::run()
{
	sf::RenderWindow window(sf::VideoMode(750, 750), "SNAKE");

	sf::Font font;
	font.loadFromFile("Freedom-10eM.ttf");

	sf::Text title("Snake", font);
	title.setCharacterSize(100);
	title.setPosition(200, 50);
	title.setFillColor(sf::Color::Black);
	title.setOutlineColor(sf::Color::White);
	title.setOutlineThickness(5);
	title.setStyle(sf::Text::Bold);

	sf::Music music;
	if (!music.openFromFile("Watery Graves (Fast Version).mp3"))
	{
		std::cout << "Error: could not load music file" << std::endl;
		exit(1);
	}

	sf::Texture texture;
	if (!texture.loadFromFile("button-icon-png-21057.png"))
	{
		std::cout << "Error: could not load texture file" << std::endl;
		exit(1);
	}
	texture.setSmooth(true);

	sf::Sprite button1;
	button1.setTexture(texture);
	button1.setScale(0.15f, 0.15f);
	button1.setPosition(150, 300);

	sf::Sprite button2;
	button2.setTexture(texture);
	button2.setScale(0.15f, 0.15f);
	button2.setPosition(500, 300);

	sf::Sprite button3;
	button3.setTexture(texture);
	button3.setScale(0.15f, 0.15f);
	button3.setPosition(325, 450);

	sf::Text single_player("Single Player", font);
	single_player.setCharacterSize(30);
	single_player.setOrigin(single_player.getGlobalBounds().getSize() / 4.f + single_player.getLocalBounds().getPosition());
	single_player.setPosition((button1.getPosition() * 0.9f) + (button1.getScale() / 2.f));
	single_player.setFillColor(sf::Color::White);

	sf::Text onevone("One on One", font);
	onevone.setCharacterSize(30);
	onevone.setOrigin(onevone.getGlobalBounds().getSize() / 15.f + onevone.getLocalBounds().getPosition());
	onevone.setPosition((button2.getPosition() * 0.9f) + (button2.getScale() / 2.f));
	onevone.setFillColor(sf::Color::White);

	sf::Text timer("timer", font);
	timer.setCharacterSize(30);
	timer.setOrigin(timer.getGlobalBounds().getSize() / 100.f + timer.getLocalBounds().getPosition());
	timer.setPosition((button3.getPosition() * 0.95f) + (button3.getScale() / 2.f));
	timer.setFillColor(sf::Color::White);

	music.setLoop(true);
	music.setVolume(25);
	music.play();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (button1.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					{
						window.close();
						music.stop();
						single_player_mode();
					}
					else if (button2.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					{
						window.close();
						music.stop();
						onevone_mode();
					}
					else if (button3.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
					{
						window.close();
						music.stop();
						timer_mode();
					}
				}
			}

		}

		window.clear();
		window.draw(title);
		window.draw(single_player);
		window.draw(onevone);
		window.draw(timer);
		window.draw(button1);
		window.draw(button2);
		window.draw(button3);
		window.display();
	}
}

void Snake::single_player_mode()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "SNAKE: SINGLE PLAYER");
	World w;
	snake_guy snake;
	Food f;
	bool play_death_sound = true;
	int loop_counter = 0;
	int food_max = 6;
	bool speed_control = false;

	sf::SoundBuffer sound;
	sound.loadFromFile("All-Star Rest Area - Super Smash Bros. Brawl.mp3");

	sf::SoundBuffer sound2;
	sound2.loadFromFile("ytmp3free.cc_dark-souls-you-died-sound-effect-youtubemp3free.org.mp3");

	sf::Sound music;
	music.setBuffer(sound);
	music.setLoop(true);
	music.setVolume(25);
	music.play();

	sf::Sound music2;
	music2.setBuffer(sound2);
	music2.setVolume(100);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		w.display_world(window);
		if ((loop_counter % 20 == 0 && f.getsize() < food_max) || f.getsize() == 0)
		{
			f.generate_food(snake);
		}
		f.display(window);
		//std::cout << "(" << snake.getXPosition() << ", " << snake.getYPosition() << ")" << std::endl;
		snake.display(window);
		//std::cout << "(" << snake.getXPosition() << ", " << snake.getYPosition() << ")" << std::endl;
		snake.move(f);
		//std::cout << "(" << snake.getXPosition() << ", " << snake.getYPosition() << ")" << std::endl;
		f.eat(snake);
		//std::cout << "(" << snake.getXPosition() << ", " << snake.getYPosition() << ")" << std::endl;
		//std::cout << "--------" << std::endl;
		if (snake.getSpeed())
		{
			if (speed_control)
			{
				speed_control = false;
			}
			else
			{
				speed_control = true;
			}
		}
		snake.game_over(window);
		if (snake.isDead() && play_death_sound)
		{
			std::cout << "Score: " << snake.getScore() << std::endl;
			music.stop();
			music2.play();
			play_death_sound = false;
		}
		window.display();
		if (speed_control)
		{
			sf::sleep(sf::milliseconds(50));
		}
		else
		{
			sf::sleep(sf::milliseconds(100));
		}
		loop_counter++;
	}
}

void Snake::onevone_mode()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "SNAKE: ONE ON ONE");
	World w;
	snake_guy snake1;
	snake_guy snake2(rand() % 10 + 5, rand() % 10 + 5);
	Food f;
	bool play_death_sound = true;
	int loop_counter = 0;
	int food_max = 12;
	bool speed_control = false;

	sf::SoundBuffer sound;
	sound.loadFromFile("Sora no Kiseki the 3rd OST - Till the Night of Glory.mp3");

	sf::SoundBuffer sound2;
	sound2.loadFromFile("ytmp3free.cc_dark-souls-you-died-sound-effect-youtubemp3free.org.mp3");

	sf::Sound music;
	music.setBuffer(sound);
	music.setLoop(true);
	music.setVolume(25);
	music.play();

	sf::Sound music2;
	music2.setBuffer(sound2);
	music2.setVolume(100);

	music.setLoop(true);
	music.setVolume(25);
	music.play();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		w.display_world(window);
		if ((loop_counter % 20 == 0 && f.getsize() < food_max) || f.getsize() == 0)
		{
			f.generate_food(snake1);
			f.generate_food(snake2);
		}
		f.display(window);
		snake1.display(window);
		snake2.display(window);
		snake1.move(f);
		snake2.move2(f);
		f.eat(snake1);
		f.eat(snake2);
		snake1.hits(snake2);
		snake2.hits(snake1);
		if (snake1.getSpeed() || snake2.getSpeed())
		{
			if (speed_control)
			{
				speed_control = false;
			}
			else
			{
				speed_control = true;
			}
		}
		snake1.game_over(window);
		snake2.game_over(window);
		if (snake1.isDead() && play_death_sound)
		{
			snake2.setDead(true);
			music.stop();
			music2.play();
			play_death_sound = false;
		}
		if (snake2.isDead() && play_death_sound)
		{
			snake1.setDead(true);
			music.stop();
			music2.play();
			play_death_sound = false;
		}
		window.display();
		if (speed_control)
		{
			sf::sleep(sf::milliseconds(50));
		}
		else
		{
			sf::sleep(sf::milliseconds(100));
		}
		loop_counter++;
	}
	std::cout << "Player 1 Score: " << snake1.getScore() << std::endl;
	std::cout << "Player 2 Score: " << snake2.getScore() << std::endl;
	if (snake1.getScore() > snake2.getScore())
	{
		std::cout << "Player 1 wins!" << std::endl;
	}
	else if (snake2.getScore() > snake1.getScore())
	{
		std::cout << "Player 2 wins!" << std::endl;
	}
	else
	{
		std::cout << "Tie!" << std::endl;
	}
}

void Snake::timer_mode()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "SNAKE: TIMER");
	World w;
	snake_guy snake;
	Food f;
	bool play_death_sound = true;
	int loop_counter = 0;
	int food_max = 6;
	int time_left = 500;
	bool speed_control = false;

	sf::Music music;
	if (!music.openFromFile("City Trial - Super Smash Bros. Ultimate.mp3"))
	{
		std::cout << "Error: could not load music file" << std::endl;
		exit(1);
	}

	sf::Music music2;
	if (!music2.openFromFile("ytmp3free.cc_dark-souls-you-died-sound-effect-youtubemp3free.org.mp3"))
	{
		std::cout << "Error: could not load music file" << std::endl;
		exit(1);
	}

	sf::Music music3;
	if (!music3.openFromFile("Hurry Up Sound Mario Brothers (Sound Effect).mp3"))
	{
		std::cout << "Error: could not load music file" << std::endl;
		exit(1);
	}

	music.setLoop(true);
	music.setVolume(25);
	music2.setVolume(100);
	music3.setVolume(100);
	music.play();

	std::cout << "Starting Time: " << time_left << std::endl;

	while (window.isOpen() && time_left > 0)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		w.display_world(window);
		if ((loop_counter % 20 == 0 && f.getsize() < food_max) || f.getsize() == 0)
		{
			f.generate_food(snake);
		}
		f.display(window);
		snake.display(window);
		snake.move(f);
		f.eat(snake);
		if (time_left == 100)
		{
			music3.play();
			std::cout << "Time Left: " << time_left << std::endl;
		}
		if (snake.getSpeed())
		{
			if (speed_control)
			{
				speed_control = false;
			}
			else
			{
				speed_control = true;
			}
		}
		snake.game_over(window);
		if (snake.isDead() && play_death_sound)
		{
			music.stop();
			music2.play();
			play_death_sound = false;
			std::cout << "Score: " << snake.getScore() << std::endl;
		}
		window.display();
		loop_counter++;
		--time_left;
		if (speed_control)
		{
			sf::sleep(sf::milliseconds(50));
		}
		else
		{
			sf::sleep(sf::milliseconds(100));
		}
	}
	std::cout << "Times Up!" << std::endl;
	std::cout << "Score: " << snake.getScore() << std::endl;
}

bool Snake::play_again()
{
	return true;
}