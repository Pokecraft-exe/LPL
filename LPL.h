#pragma once
#include "LPL_result.h"
#include <iostream>
#include <fstream>
#include <exception>

class LPL {
public:
	std::ifstream file;
	vector<string> schemes = {};
	vector<map<string, vector<string>>> properties = {};
	vector<map<string, vector<string>>> definitions = {};
	vector<map<string, vector<string>>> patterns = {};
private:
	void loadSchemes() {
		string line = "";
		while (!file.eof()) {
			std::getline(file, line);
			if (line.find("//", 0) == string::npos) {
				if (line[0] == '(') {
					schemes.push_back(line.substr(1, line.size()));
				}
			}
		}
	}
	void loadPropertiesForScheme(string scheme) {
		string line = "";
		bool schemeStarted = false;
		string currentScheme = "";

		while (!file.eof()) {
			std::getline(file, line);
			if (line.find("//", 0) == string::npos) {
				if (line[0] == '(') {
					currentScheme = line.substr(1, line.size());
					if (currentScheme == scheme) {
						schemeStarted = true;
						properties.push_back(map<string, vector<string>>());
					}
				}
				if (schemeStarted) {
					if (line[0] == '"')

				}
			}
		}
	}
public:
	bool loadScript(string fileToRead) {
		file = std::ifstream(fileToRead);
		if (file.good()) {
			loadSchemes();
			return true;
		}
		else {
			throw std::invalid_argument("file can't open");
			return false;
		}
	}
	
};