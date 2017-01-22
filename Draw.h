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

	//game.sparks[0].sprite.setPosition(0, 0);
	//(*window).draw(game.sparks[0].sprite);

/*	for (int i = 0; i < game.hits.size(); i++) {
		sf::Sprite s = game.sparks[game.hits[i].second].sprite;
		s.setPosition((game.WINDOW_SIZE.first / game.NOTES) * game.curr_song[i].pitch, 40.0);
		(*window).draw(s);
	}*/
}

void draw_menu(Game game, sf::RenderWindow * window) {
	sf::Sprite play = game.play_button.sprite;
	play.setPosition(50, 50);
	(*window).draw(play);
}