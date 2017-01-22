#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

void draw_frame(Game game, sf::RenderWindow * window) {
	for (int i = 0; i < game.curr_song.size(); i++) {
		sf::Sprite s = game.bar_body.sprite;
		s.scale(sf::Vector2f(1, (game.curr_song[i].end - game.curr_song[i].start) / 200.0));
		s.setPosition((game.WINDOW_SIZE.first / game.NOTES) * game.curr_song[i].pitch, game.WINDOW_SIZE.second - (game.curr_song[i].end - game.tick));
		(*window).draw(s);
	}

	sf::Text score_text;
	score_text.setFont(game.font);
	score_text.setString("Score: " + std::to_string(game.score));
	score_text.setPosition(5, 5);
	score_text.setCharacterSize(36);
	(*window).draw(score_text);

	sf::Text accuracy_text;
	accuracy_text.setFont(game.font);
	accuracy_text.setString("A: " + (std::to_string((int)(game.accuracy))));
	accuracy_text.setPosition(game.WINDOW_SIZE.first - 240, 5);
	accuracy_text.setCharacterSize(36);
	(*window).draw(accuracy_text);

	std::cout << game.wrong_indexes.size();

	std::vector<sf::Sprite> esses;
	for (int i = 0; i < game.wrong_indexes.size(); i++) {
		esses.push_back(game.red_x.sprite);
		int xPos = ((float)(game.WINDOW_SIZE.first) / game.NOTES) * (game.wrong_indexes[i]);
		int yPos = game.WINDOW_SIZE.second - 80;
		esses[i].setPosition(xPos-40, yPos);
		(*window).draw(esses[i]);
	}

	//game.sparks[0].sprite.setPosition(0, 0);
	//(*window).draw(game.sparks[0].sprite);

	/*	for (int i = 0; i < game.hits.size(); i++) {
	sf::Sprite s = game.sparks[game.hits[i].second].sprite;
	s.setPosition((game.WINDOW_SIZE.first / game.NOTES) * game.curr_song[i].pitch, 40.0);
	(*window).draw(s);
	}*/
}

void draw_meenu(Game game, sf::RenderWindow * window) {
	sf::Sprite easy = game.easy_button.sprite;
	easy.setPosition(105, 100);
	(*window).draw(easy);

	sf::Sprite med = game.med_button.sprite;
	med.setPosition(755, 100);
	(*window).draw(med);

	sf::Sprite hard = game.hard_button.sprite;
	hard.setPosition(1410, 100);
	(*window).draw(hard);

	sf::Text score_text;
	score_text.setFont(game.font);
	score_text.setString("Please select a difficulty." );
	score_text.setPosition(50, 5);
	score_text.setCharacterSize(80);
	(*window).draw(score_text);
}

void draw_over(Game game, sf::RenderWindow * window) {
	sf::Text score_text;
	score_text.setFont(game.font);
	score_text.setString("Game Over");
	score_text.setPosition(5, 5);
	score_text.setCharacterSize(125);
	(*window).draw(score_text);

	sf::Text stat;
	stat.setFont(game.font);
	stat.setString("Score: " + game.score);
	stat.setPosition(5, 50);
	stat.setCharacterSize(64);
	(*window).draw(stat);
	
	sf::Text stat2;
	stat2.setFont(game.font);
	stat2.setString("Press the left most key to play again.");
	stat2.setPosition(5, 100);
	stat2.setCharacterSize(64);
	(*window).draw(stat2);

}