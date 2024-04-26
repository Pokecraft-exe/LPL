#include <string>

enum class functions {
	none = 0,
	map,
	list,
	ref,
};

/*struct schemeProperty {
	bool multiline;
	std::vector<std::map<std::string, std::string>> definitions;
};*/

class property {
private:
	std::string _name;
	functions _function = functions::none;
	std::string _arguments = "";
public:
	property(std::string name, functions function, std::string args) {
		_name = name;
		_function = function;
		_arguments = args;
	}
	property(std::string name) {
		_name = name;
	}
	std::string name() {
		return _name;
	}
	operator std::string() {
		return _name;
	}
	std::string compile() {
		if (_function == functions::ref) {
			return "\t" + _arguments + "* " + _name + " = nullptr;\n";
		}
		if (_function == functions::map) {
			return "\tstd::map<std::string, std::string> " + _name + " = {};\n";
		}
		if (_function == functions::list) {
			return "\tstd::vector<" + _arguments + "> " + _name + " = {};\n";
		}
		return "\tstd::string " + _name + " = \"\";\n";
	}
	std::string include() {
		return _arguments;
	}
};