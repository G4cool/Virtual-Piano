#include <SFML/Graphics.hpp>
#include <iostream>
#include "Draw.h"
#include "Game.h"
#include "Melody.h"

int main(){
	Game game = Game();
	sf::Clock frame_timer;
	sf::RenderWindow window(sf::VideoMode(game.WINDOW_SIZE.first, game.WINDOW_SIZE.second), "Game");

	//std::cout << std::to_string(seperate("br.csv")[0][0].pitch);
	std::pair < std::vector < std::vector<Note>>, int> data = seperate("br.csv");
	
	game.curr_song = data.first[0];
	

	game.BPM = data.second;
	std::cout << game.BPM;

	//game.populate_song();
	game.load_textures();
	game.populate_map();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::A) {
					game.inputs[0] = true;
				}
				if (event.key.code == sf::Keyboard::S) {
					game.inputs[1] = true;
				}
				if (event.key.code == sf::Keyboard::D) {
					game.inputs[2] = true;
				}
				if (event.key.code == sf::Keyboard::F) {
					game.inputs[3] = true;
				}
				if (event.key.code == sf::Keyboard::G) {
					game.inputs[4] = true;
				}
			}
			if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::A) {
					game.inputs[0] = false;
				}
				if (event.key.code == sf::Keyboard::S) {
					game.inputs[1] = false;
				}
				if (event.key.code == sf::Keyboard::D) {
					game.inputs[2] = false;
				}
				if (event.key.code == sf::Keyboard::F) {
					game.inputs[3] = false;
				}
				if (event.key.code == sf::Keyboard::G) {
					game.inputs[4] = false;
				}
			}
		}

		sf::Time elapsed = frame_timer.getElapsedTime();
		if (elapsed.asMilliseconds() >= 60) {
			frame_timer.restart();
			window.clear();

			game.tick += ((elapsed.asMilliseconds() * game.BPM * 256) / 60000);
		//	std::cout << std::to_string(game.tick) + "\n";
			if (game.stage == "MENU") {
				
				if (game.inputs[0]) {
					game.tick = -256*4;
					game.stage = "GAME";
				}
				draw_menu(game, &window);
			}
			if (game.stage == "GAME") {
				
				if (!game.check_inputs()) {
					//error
				}
				else{
					if (game.hits.size() > 0) {
						game.score += 150 * game.hits.size();
					}
				}

				if (game.accuracy < -1) {
					game.stage = "OVER";
				}

				draw_frame(game, &window);
			}
			
			window.display();
		}
	}

	return 0;
}