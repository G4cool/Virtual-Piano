#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include "Game.h"

using namespace std;

bool byTime(const Note & eOne, const Note & eTwo) {
	return eOne.start < eTwo.start;
}

bool byPitch(const Note & eOne, const Note & eTwo) {
	return eOne.pitch < eTwo.pitch;
}

vector<double> scores(vector<vector<double>> inputs, vector<double> maxes) {
	vector<double> outputs;
	for (vector<double> inps : inputs) {
		outputs.push_back(0.4*inps[0] / maxes[0] + 0.4*inps[1] / maxes[1] + 0.2*inps[2] / maxes[2]);
	}
	return outputs;
}
//vector <vector<pair<pair<int, int>, int>> changeKey(vector < vector<pair<pair<int, int>, int>>original, int change) {
//	for (int i = 0; i < )
//}
pair<vector<vector<Note>>, int> seperate(string filename) {
	ifstream f(filename);

	string line;

	vector<vector<string>> data;

	while (getline(f, line)) {
		vector<string> row;
		stringstream iss(line);
		string val;

		while (getline(iss, val, ',')) {
			val.erase(remove(val.begin(), val.end(), ' '), val.end());
			row.push_back(val);
		}
		data.push_back(row);


	}


	int LOW = 36;
	int RANGE = 49;
	int HIGH = LOW + RANGE;
	bool multiple = true;
	int head;
	int highest = 0;
	int lowest = 10000;
	stringstream(data[0][5]) >> head;
	int bpm = 0;
	int tempo = 0;
	double conversion = 0;
	vector<vector<pair<pair<int, int>, int>>> outputs;
	vector<pair<pair<int, int>, int>> output;
	vector<double> note_rate;
	vector<double> mean;
	vector<double> lcr_values;
	vector<int> distinct_notes;
	int sum = 0;
	int count = 0;
	int second_count = 0;
	double last_time = 1;

	if (data[0][3] == "0") multiple = false;

	for (int i = 0; i < data.size(); i++) {
		if (data[i][2] == "Tempo") {
			stringstream(data[i][3]) >> tempo;
			conversion = (double)tempo / (1000000.0*head);
			bpm = tempo * 240 / 1000000;
		}
		//tempo = n/1000000 seconds per quarter note
		// 1/256 of a beat 
		//given clock time
		//clocktime/head/256
		//head = clock time per quarter note
		//conversion = seconds per clock time
		else if (data[i][2] == "End_track") {
			if(output.size()!=0){
				outputs.push_back(output);
				note_rate.push_back(second_count);
				if (second_count != 0)
					mean.push_back(sum / second_count);
				else
					mean.push_back(-1);
				}

			sum = 0;
			count = 0;
			second_count = 0;
		}
		else if (data[i][2] == "Start_track")
			output.clear();
		else if (data[i][2] == "Note_on_c") {
			for (int j = i + 1; j < data.size(); j++) {
				count++;
				if (data[i][0] == data[j][0] && data[j][3] == data[i][3] && data[j][4] == data[i][4] && (data[j][5] == "0" || data[j][2] == "Note_off_c")) {
					second_count++;
					int start_time;
					stringstream(data[i][1]) >> start_time;
					int end_time;
					stringstream(data[j][1]) >> end_time;
					int note_val;
					stringstream(data[i][4]) >> note_val;
					sum += note_val;
					if (note_val > highest) highest = note_val;
					if (note_val < lowest) lowest = note_val;

					output.push_back(make_pair(make_pair((int)(start_time * 256 / head), (int)(end_time * 256 / head)), note_val));

					last_time = end_time*conversion;
					data.erase(data.begin() + j);
					data.erase(data.begin() + i);
					i--;
					break;

				}
				
			}
		}
	}
//	int range = highest - lowest;
//	if (range > RANGE) {
//		if ()
//	}
	vector<vector<double>> evaluations;
	for (int i = 0; i < outputs.size(); i++) {
		vector<double> evaluation;
		evaluation.push_back(note_rate[i] / last_time);
		int lcr = 0;
		vector<int> distinct;
		double cursor = mean[i];
		for (int j = 0; j < outputs[i].size(); j++) {
			int next_note = outputs[i][j].second;


			if ((next_note>mean[i] && cursor<mean[i]) || (next_note<mean[i] && cursor>mean[i]))
				lcr++;
			cursor = next_note;


			if (find(distinct.begin(), distinct.end(), cursor) == distinct.end())
				distinct.push_back(next_note);





		}
		evaluation.push_back(lcr / mean[i]);
		evaluation.push_back(distinct.size());
		distinct.clear();
		evaluations.push_back(evaluation);

	}

	vector<double> maxes;
	maxes.push_back(-1.0);
	maxes.push_back(-1.0);
	maxes.push_back(-1.0);
	for (vector<double> evals : evaluations) {
		if (evals[0] > maxes[0]) maxes[0] = evals[0];
		if (evals[1] > maxes[1]) maxes[1] = evals[1];
		if (evals[2] > maxes[2]) maxes[2] = evals[2];
	}
	vector<double> score = scores(evaluations, maxes);
	int argMax = distance(score.begin(), max_element(score.begin(), score.end()));
	score.erase(score.begin() + argMax);
	vector<pair<pair<int, int>, int>> melody = outputs[argMax];
	outputs.erase(outputs.begin() + argMax);

	int secondMax = distance(score.begin(), max_element(score.begin(), score.end()));
	vector<pair<pair<int, int>, int>> harmony = outputs[secondMax];

	vector<pair<pair<int, int>, int>> full_out;

	full_out.reserve(harmony.size() + melody.size());
	full_out.insert(full_out.end(), melody.begin(), melody.end());
	full_out.insert(full_out.end(), harmony.begin(), harmony.end());
	cout << "Test";
	for (pair<pair<int, int>, int> val : full_out) {
		cout << "\n" << val.first.first << ", " << val.first.second << ", " << val.second;
	}
	cout << "\n len: " << full_out.size();

	vector<Note> full_song;
	vector<Note> melody_song;
	vector<Note> harmony_song;
	vector<vector<Note>> final_export;


	for (int i = 0; i < full_out.size(); i++) {
		full_song.push_back(Note(full_out[i].first.first, full_out[i].first.second, full_out[i].second));
	}

	for (int i = 0; i < melody.size(); i++) {
		melody_song.push_back(Note(melody[i].first.first, melody[i].first.second, melody[i].second));
	}

	for (int i = 0; i < harmony.size(); i++) {
		harmony_song.push_back(Note(harmony[i].first.first, harmony[i].first.second, harmony[i].second));
	}

	sort(full_song.begin(), full_song.end(), byPitch);

	while ((full_song[full_song.size() - 1].pitch - full_song[0].pitch) > 49) {
		full_song.erase(full_song.begin());
	}

	sort(full_song.begin(), full_song.end(), byTime);

	final_export.push_back(full_song); final_export.push_back(melody_song); final_export.push_back(harmony_song);
	
	return make_pair(final_export, bpm);


}
