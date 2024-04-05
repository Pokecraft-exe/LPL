/**
 *
 * Leviathan Parse Language is the ulimate way to parse anything
 * the easy API makes parsing a lot easier
 *
 **/

#include "LPL.h"

void main() {
	LPL parser;
	if (parser.loadScript("Texte.txt")) std::cout << "opened\n";

	for (auto s : parser.schemes) std::cout << s << std::endl;

	return;
}