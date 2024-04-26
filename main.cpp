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

	if (argc != 2) {
		std::cout << "LPL compiler must have only one argument: [file]" << std::endl;
	}

	parser.loadScript(argv[1]);
	parser.compileAll();

	return;
}