#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Draw.h"
#include "Game.h"
#include "Melody.h"



/*std::vector<Note> read_song(std::string filename) {
	std::ifstream f(filename);

	std::string line;

	std::vector<Note> song;

	while (std::getline(f, line)) {
		std::cout << line;
		if (line.find("Note_on_c") != std::string::npos) {
			line = line.substr(line.find(",") + 2);
			int time = stoi(line.substr(line.find(",") + 2, line.find(",")));
			line = line.substr(line.find("Note_on_c") + 10);
			line = line.substr(line.find(",") + 2);
			int note = stoi(line.substr(0, line.find(",")));
			song.push_back(Note(time, time + 50, note));
		}
	}
}*/

int main() {
	Game game = Game();
	sf::Clock frame_timer;
	

	//std::cout << std::to_string(seperate("br.csv")[0][0].pitch);
	std::pair < std::vector < std::vector<Note>>, int> data = seperate("deb_menu.csv");

	game.curr_song = data.first[0];

//	read_song("deb_menu.csv");

	game.BPM = 75;

//	game.populate_song();
	game.load_textures();
	game.populate_map();

	sf::RenderWindow window(sf::VideoMode(game.WINDOW_SIZE.first, game.WINDOW_SIZE.second), "Game", sf::Style::Fullscreen);

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
				if (game.inputs[0]) { //easy
					game.tick = -256 * 4;
					game.BPM = 60;
					game.cutoff = 60;
					game.stage = "GAME";
				}else if (game.inputs[1]) { //med
					game.tick = -256 * 4;
					game.BPM = 80;
					game.cutoff = 80;
					game.stage = "GAME";
				}else if (game.inputs[2]) { //hard
					game.tick = -256 * 4;
					game.BPM = 100;
					game.cutoff = 93;
					game.stage = "GAME";
				}

				draw_meenu(game, &window);
			}
			if (game.stage == "GAME") {

				if (!game.check_inputs()) {
					//error
				}
				else {
					if (game.hits.size() > 0) {
						game.score += 150 * game.hits.size();
					}
				}

				if (game.accuracy < game.cutoff) {
					game.stage = "OVER";
				}

				draw_frame(game, &window);
			}
			if (game.stage == "OVER") {
				draw_over(game, &window);
			}

			window.display();
		}
	}

	return 0;
}