#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

class Asset {
public:
	void init(sf::Texture, sf::Sprite, std::pair<int, int>);
	sf::Texture texture;
	sf::Sprite sprite;
	std::pair<int, int> size;
};

void Asset::init(sf::Texture t, sf::Sprite spr, std::pair<int, int> s) {
	texture = t;
	sprite = spr;
	size = s;
}

class Note {
public:
	Note(int, int, int);
	int start = 0;
	int end = 0;
	int pitch = 0;
	bool lost = false;
};

Note::Note(int s, int e, int p) {
	start = s;
	end = e;
	pitch = p;
}

class Game {
public:
	Game();

	int BPM = 160; //might change with different songs
	const int NOTES = 88; //notes on one screen (piano size)
	const int KINDNESS = 16;
	std::pair<int, int> WINDOW_SIZE = std::make_pair(1000, 500);

	//textures
	Asset bar_body;
	Asset easy_button;
	Asset med_button;
	Asset hard_button;
	Asset help_button;
	Asset play_button;
	Asset red_x;
	sf::Font font;
	std::vector<Asset> sparks;

	//session vars
	int tick = -256 * 4; //give 8 beats to prepare
	std::vector<Note> curr_song;
	std::vector<bool> inputs;
	std::vector<std::vector<int>> map;
	std::vector<std::pair<int, int>> hits;
	std::string stage = "MENU";
	float hit_frames = 50.0;
	float total_frames = 50.0;

	//game vars
	int score = 0;
	float accuracy = 0;

	//methods
	void populate_song();
	void populate_map();
	int load_textures();
	bool check_inputs();

};

Game::Game() {
	for (int i = 0; i < NOTES; i++) {
		inputs.push_back(false);
	}
}

int Game::load_textures() {
	if (!bar_body.texture.loadFromFile("images/bar_body.png")) {
		exit(0);
	}
	bar_body.sprite.setTexture(bar_body.texture);
	bar_body.sprite.setScale(sf::Vector2f(((float)(WINDOW_SIZE.first)/NOTES)/bar_body.texture.getSize().x, 200.0/bar_body.texture.getSize().y));
	if (!play_button.texture.loadFromFile("images/play_button.png")) {
		exit(0);
	}
	play_button.sprite.setTexture(play_button.texture);
	
	if (!font.loadFromFile("images/font.otf"))
	{
		exit(0);
	}
	/*if (!med_button.texture.loadFromFile("images/medium_button.png")) {
		exit(0);
	}
	if (!hard_button.texture.loadFromFile("images/hard_button.png")) {
		exit(0);
	}*/

	/*for (int i = 0; i <= 116; i += 4) {
		std::string filename = "images/Sparks" + std::to_string(i) + ".png";
		Asset a; sparks.push_back(a);
		if (!sparks[i/4].texture.loadFromFile(filename)) {
			exit(0);
		}
		sparks[i/4].sprite.setTexture(sparks[i/4].texture);
	}*/
	
}


void Game::populate_song() { //developer
	curr_song.push_back(Note(0, 128, 0));
	curr_song.push_back(Note(256, 256*1.5, 0));
	curr_song.push_back(Note(256*2, 256*2.5, 3));
	curr_song.push_back(Note(256 * 3, 256 * 3.5, 3));
	curr_song.push_back(Note(256 * 4, 256 * 4.5, 4));
	curr_song.push_back(Note(256 * 5, 256 * 5.5, 4));
	curr_song.push_back(Note(256 * 6, 256 * 8, 3));
}

void Game::populate_map() {
	std::vector<int> tick_row;
	std::cout << curr_song[curr_song.size() - 1].start;
	for (int i = 0; i < curr_song[curr_song.size()-1].end; i++) {
		tick_row.clear();
		for (int j = 0; j < inputs.size(); j++) {
			tick_row.push_back(-1);
		}
		for (int j = 0; j < curr_song.size(); j++) {
			if ((curr_song[j].start < i) && (curr_song[j].end > i)) {
				tick_row[curr_song[j].pitch] = 1;
			}
			if (abs(curr_song[j].start - i) < KINDNESS*4 || abs(curr_song[j].end - i) < KINDNESS*4) {
				tick_row[curr_song[j].pitch] = 0;
			}
		}
		map.push_back(tick_row);
	}
}

bool Game::check_inputs() {
	hits.clear();
	accuracy = 100 * hit_frames / total_frames;

	if (tick >= 0) {
		//std::cout << "Notes first: " + std::to_string(notes_seen.first) + " and notes second: " + std::to_string(notes_seen.second);
		for (int i = 0; i < inputs.size(); i++) {
			if (inputs[i]) { //input down
				if (map[tick][i] == 1 || map[tick][i] == 0) { //yes notes
					hit_frames++; total_frames++;

					bool did_exist = false;
					for (int m = 0; m < hits.size(); m++) {
						if (hits[m].first == i) {
							did_exist = true;
							hits[m].second++;
						}
					}
					if (!did_exist) {
						hits.push_back(std::make_pair(i, 0));
					}
				} else {
					total_frames++;
					return false;
				}
			} else {
				if (map[tick][i] == -1 || map[tick][i] == 0) { //no notes
					//good
				}
				else {
					total_frames++;
					return false;
				}
			}
		}
	}
	
	return true;
	
}