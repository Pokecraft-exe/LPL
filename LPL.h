#pragma once
#include "LPL_result.h"
#include "property.h"
#include <iostream>
#include <fstream>
#include <exception>

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

class LPL {
public:
	std::ifstream file;
	vector<string> schemes = {};
	map<string, vector<property>> properties = {};
	map<string, vector<string>> definitions = {};
	map<string, string> patterns = {};
private:
	void loadProperties() {
		string line = "";
		bool schemeStarted = false;
		bool patternStarted = false;
		string currentScheme = "";

		while (!file.eof()) {
			std::getline(file, line);
			if (line.find("//", 0) == string::npos) {
				if (line[0] == '(') {
					currentScheme = line.substr(1, line.size());
					schemeStarted = true;
					properties[currentScheme] = vector<property>();
					schemes.push_back(line.substr(1, line.size()));
				}
				if (schemeStarted && !patternStarted) {
					if (line[0] == '"') {
						string currentProperty = line.substr(1, line.size() - 2);
						properties[currentScheme].push_back(currentProperty);
					}
					else if (line.find("map(") != string::npos) {
						int posStart = line.find('\"') + 1;
						int posEnd = line.rfind("\"") - posStart;
						property currentProperty(line.substr(posStart, posEnd), functions::map, {});
						properties[currentScheme].push_back(currentProperty);
					}
					else if (line.find("list(") != string::npos) {
						int posStart = line.find('\"') + 1;
						int posEnd = line.rfind("\"") - posStart;
						int posArgStart = line.find("\'") + 1;
						int posArgEnd = line.rfind("\'") - posArgStart;
						property currentProperty(line.substr(posStart, posEnd), functions::list, { line.substr(posArgStart, posArgEnd) });
						properties[currentScheme].push_back(currentProperty);
					}
					else if (line.find("ref(") != string::npos) {
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
	std::string identify(std::string scheme) {
		std::string func = "\
\t\tstd::vector<std::string> Tpattern = LPLtokenize(pattern);\n\
\t\tstd::vector<std::string> toIdentify = LPLtokenize(std::string(begin, end), false);\n\
\t\tstd::vector<std::vector<std::string>> TTpattern = LPLtokenize(Tpattern);\n\t\t"+scheme+" n;\n\
\t\tidentification_result result;\n\
\t\tresult.success = false;\n\
\t\tint i = 0;\n\
\t\tfor (auto token : TTpattern) {\n\
\t\t\tif (token[0] == \"default\") {\n\
\t\t\t\tif (toIdentify.size() < TTpattern.size()) {\n\
\t\t\t\t\tstring def = \"\";\n\
\t\t\t\t\tdef = *(std::find(token.begin(), token.end(), \"=\") + 1);\n\
\t\t\t\t\tstring into = \"\";\n\
\t\t\t\t\tinto = *(std::find(token.begin(), token.end(), \"\\\"\") + 1);\n\
\t\t\t\t\ttoIdentify.insert(toIdentify.begin() + i, def);\n\
\t\t\t\t\tn.push(into, def);\n\
\t\t\t\t\ti++;\n\
\t\t\t\t\tcontinue;\n\
\t\t\t\t}\n\
\t\t\t\tif (toIdentify.size() >= TTpattern.size()) {\n\
\t\t\t\t\tstring into = \"\";\n\
\t\t\t\t\tinto = *(std::find(token.begin(), token.end(), \"\\\"\") + 1);`\n\
\t\t\t\t\tn.push(into, toIdentify[i]);\n\
\t\t\t\t\ti++;\n\
\t\t\t\t\tcontinue;\n\
\t\t\t\t}\n\
\t\t\t\treturn result;\n\
\t\t\t}\n\
\t\t\tif (token[0] == \"or\") {\n\
\t\t\t\tstd::cout << \"or() : line \" << std::to_string(__LINE__) << \"; \\n\";\n\
\t\t\t\ti++;\n\
\t\t\t\tcontinue;\n\
\t\t\t}\n\
\t\t\tif (token[0] == \".\" && token[1] == \".\" && token[2] == \".\") {\n\
\t\t\t\tstd::cout << \"...() : line \" << std::to_string(__LINE__) << \";\\n\";\n\
\t\t\t\ti++;\n\
\t\t\t\tcontinue;\n\
\t\t\t}\n\
\t\t\tif (token[0] == \"numeric\") {\n\
\t\t\t\tif (token.size() == 3 &&\n\
\t\t\t\t\ttoken[1] == \"(\" &&\n\
\t\t\t\t\ttoken[2] == \")\") {\n\
\t\t\t\t\tstd::string numeric = \"0123456789\";\n\
\t\t\t\t\tfor (auto c : toIdentify[i]) {\n\
\t\t\t\t\t\tif (numeric.find(c) == std::string::npos) {\n\
\t\t\t\t\t\t\treturn result;\n\
\t\t\t\t\t\t}\n\
\t\t\t\t\t}\n\
\t\t\t\t\ti++;\n\
\t\t\t\t\tcontinue;\n\
\t\t\t\t}\n\
\t\t\t\treturn result;\n\
\t\t\t}\n\
\t\t\tif (token[0] == \"alpha\") {\n\
\t\t\t\tif (token.size() == 3 &&\n\
\t\t\t\t\ttoken[1] == \"(\" &&\n\
\t\t\t\t\ttoken[2] == \")\") {\n\
\t\t\t\t\tstd::string alpha = \"azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN\";\n\
\t\t\t\t\tfor (auto c : toIdentify[i]) {\n\
\t\t\t\t\t\tif (alpha.find(c) == std::string::npos) {\n\
\t\t\t\t\t\t\treturn result;\n\
\t\t\t\t\t\t}\n\
\t\t\t\t\t}\n\
\t\t\t\t\ti++;\n\
\t\t\t\t\tcontinue;\n\
\t\t\t\t}\n\
\t\t\t\treturn result;\n\
\t\t\t}\n\
\t\t\tif (token[0] == \"alphanum\") {\n\
\t\t\t\tif (token.size() == 3 &&\n\
\t\t\t\t\ttoken[1] == \"(\" &&\n\
\t\t\t\t\ttoken[2] == \")\") {\n\
\t\t\t\t\tstd::string alphanum = \"azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN0123456\";\n\
\t\t\t\t\tfor (auto c : toIdentify[i]) {\n\
\t\t\t\t\t\tif (alphanum.find(c) == std::string::npos) {\n\
\t\t\t\t\t\t\treturn result;\n\
\t\t\t\t\t\t}\n\
\t\t\t\t\t}\n\
\t\t\t\t\ti++;\n\
\t\t\t\t\tcontinue;\n\
\t\t\t\t}\n\
\t\t\t\treturn result;\n\
\t\t\t};\n\
\t\t\tif (token[0] == \"character\") {\n\
\t\t\t\tint num = -1;\n\
\t\t\t\twhile (toIdentify[i][num++] != 0);\n\
\t\t\t\tif (i > 1) return result;\n\
\t\t\t\ti++;\n\
\t\t\t\tcontinue;\n\
\t\t\t}\n\
\t\t\tif (token[0] == \"\\\"\") {\n\
\t\t\t\tn.push(token[1], toIdentify[i]);\n\
\t\t\t\i++;\n\
\t\t\t\tcontinue;\n\
\t\t\t}\n\
\t\t\tif (toIdentify[i] != token[0]) {\n\
\t\t\t\treturn result;\n\
\t\t\t}\n\
\t\t\ti++;\:n\
\t\t}\n\
\t\tresult.result = &n;\n\
\t\tresult.success = true;\n\
\t\treturn result;\n\
\t}\n";
		return func;
	}

public:
	bool loadScript(string fileToRead) {
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

	void compileScheme(string scheme) {
		string header = "\
@pragma once\n\
#include <string>\n\
#include <vector>\n\
#include <map>\n\
#include \"LPL.h\"\n\
  /**********************************/\n\
 /* Code generated by LPL compiler */\n\
/**********************************/\n\n\
std::vector<std::string> LPLtokenize(std::string str, bool pushSpace = true);\n\
std::vector<std::vector<std::string>> LPLtokenize(std::vector<std::string> str);\n\n";

		string code = "";
		code += "struct " + scheme + "{\n\tstatic std::string pattern;\n";
		for (auto pp : properties[scheme]) {
			code += pp.compile();
		}
		code += "\t" + scheme + "(){}\n";
		code += "\t void push(std::string parameter, string __value);\n";

		code += "\tusing identification_result = struct __result__{\n\
\t\tvariable * result;\n\
\t\tbool success;\n\
\t\t__result__() {};\n\
\t};\n";

		code += "\tstatic identification_result identify(string::iterator begin, string::iterator end);\n";
		code += "};\n\n";

		std::ofstream headerFile("LPL_" + scheme + ".h");
		
		headerFile << header + code;

		headerFile.close();

		std::ofstream codeFile("LPL_" + scheme + ".cpp");

		codeFile << "#include \"LPL_"+scheme+".h\"\n\
  /**********************************/\n\
 /* Code generated by LPL compiler */\n\
/**********************************/\n\n";

		codeFile << "std::string " + scheme + "::pattern = " + patterns[scheme] + ";\n";
		codeFile << "void "+scheme+"::push(std::string parameter, string __value) {\n";
			
		for (auto& p : properties[scheme]) {
			if (p.name().find("(") == std::string::npos)
				codeFile << "\tif (parameter == \"" + p.name() + "\") " + p.name() + " = __value;\n";
		}

		codeFile << "}\n";

		codeFile << "identification_result identify(string::iterator begin, string::iterator end) {\n" << identify(scheme) << std::endl;

		codeFile.close();
	}
	
};