#include "LPL.h"

void LPL::loadProperties() {
	std::string line = "";
	bool schemeStarted = false;
	bool patternStarted = false;
	std::string currentScheme = "";

	while (!file.eof()) {
		std::getline(file, line);
		if (line.find("//", 0) == std::string::npos) {
			if (line[0] == '(') {
				currentScheme = line.substr(1, line.size());
				schemeStarted = true;
				properties[currentScheme] = std::vector<property>();
				schemes.push_back(line.substr(1, line.size()));
			}
			if (schemeStarted && !patternStarted) {
				if (line[0] == '"') {
					std::string currentProperty = line.substr(1, line.size() - 2);
					properties[currentScheme].push_back(currentProperty);
				}
				else if (line.find("map(") != std::string::npos) {
					int posStart = line.find('\"') + 1;
					int posEnd = line.rfind("\"") - posStart;
					property currentProperty(line.substr(posStart, posEnd), functions::map, {});
					properties[currentScheme].push_back(currentProperty);
				}
				else if (line.find("list(") != std::string::npos) {
					int posStart = line.find('\"') + 1;
					int posEnd = line.rfind("\"") - posStart;
					int posArgStart = line.find("\'") + 1;
					int posArgEnd = line.rfind("\'") - posArgStart;
					property currentProperty(line.substr(posStart, posEnd), functions::list, { line.substr(posArgStart, posArgEnd) });
					properties[currentScheme].push_back(currentProperty);
				}
				else if (line.find("ref(") != std::string::npos) {
					int posStart = line.find('\"') + 1;
					int posEnd = line.rfind("\"") - posStart;
					int posArgStart = line.find('\'') + 1;
					int posArgEnd = line.rfind('\'') - posArgStart;
					property currentProperty(line.substr(posStart, posEnd), functions::ref, { line.substr(posArgStart, posArgEnd) });
					properties[currentScheme].push_back(currentProperty);
				}
			}
			if (line[0] == ')') {
				schemeStarted = false;
				patternStarted = false;
			}
			if (patternStarted) {
				patterns[currentScheme] += "\"" + ReplaceAll(line, "\"", "\\\"") + "\"";
			}
			if (schemeStarted && line[0] == '=' && line[1] == '=') {
				patternStarted = true;
			}
		}
	}
}
std::string LPL::identify(std::string scheme) {
	std::ifstream src("Source.txt");
	std::string func = slurp(src);
	func = ReplaceAll(func, "%s", scheme);
	return func;
}

bool LPL::loadScript(std::string fileToRead) {
	file = std::ifstream(fileToRead);
	if (file.good()) {
		loadProperties();
		return true;
	}
	else {
		throw std::invalid_argument("file can't open");
		return false;
	}
}

void LPL::compileAll() {
	for (std::string s : schemes) {
		compileScheme(s);
	}
}
void LPL::compileScheme(std::string scheme) {
	std::string header = sHEADER;

	std::string code = "";
	code += "struct " + scheme + "{tstatic std::string pattern;\n";
	for (auto pp : properties[scheme]) {
		code += pp.compile();
	}
	code += "\t" + scheme + "(){}\n";
	code += "\t void push(std::string parameter, string __value);\n";

	code += sRESULT_STRUCT;

	code += "\tstatic identification_result identify(string::iterator begin, string::iterator end);\n";

	code += "};\n";

	std::ofstream headerFile("LPL_" + scheme + ".h");

	headerFile << header + code;

	headerFile.close();

	std::ofstream codeFile("LPL_" + scheme + ".cpp");

	codeFile << "#include \"LPL_" + scheme + ".h\"\n  /**********************************/\n/* Code generated by LPL compiler */\n/**********************************/\n";

	codeFile << "std::string " + scheme + "::pattern = " + patterns[scheme] + ";\n";
	codeFile << "void " + scheme + "::push(std::string parameter, std::string __value) {\n";

	for (auto& p : properties[scheme]) {
		if (p.name().find("(") == std::string::npos)
			codeFile << "\tif (parameter == \"" + p.name() + "\") " + p.name() + " = __value;\n";
	}

	codeFile << "}\n";

	codeFile << "identification_result " + scheme + "::identify(std::string::iterator begin, std::string::iterator end) {\n" << identify(scheme) << std::endl;

	codeFile.close();
}