/**
 *
 * Leviathan Parse Language is the ulimate way to parse anything
 * the easy API makes parsing a lot easier
 *
 **/

#include "LPL.h"

string tests[3]{
	"int a = 0",
	"a = 0",
	"hello worsld"
};

void main(int argc, char** argv) {
	LPL parser;

	parser.loadScript("Texte.txt");

	parser.compileScheme(parser.schemes[0]);

	return;
}