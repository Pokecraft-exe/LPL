/**
 *
 * Leviathan Parse Language is the ulimate way to parse anything
 * the easy API makes parsing a lot easier
 *
 **/

#include "LPL.h"

std::string slurp(std::ifstream& in) {
	std::ostringstream sstr;
	sstr << in.rdbuf();
	return sstr.str();
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

void main(int argc, char** argv) {
	LPL parser;

	if (argc != 2) {
		std::cout << "LPL compiler must have only one argument: [file]" << std::endl;
		return;
	}

	parser.loadScript(argv[1]);
	parser.compileAll();

	return;
}