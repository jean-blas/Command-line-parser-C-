//
// Created by jimbert on 2/3/20.
//

#ifndef COMMAND_LINE_PARSER_C__CLPARSER_H
#define COMMAND_LINE_PARSER_C__CLPARSER_H

#include "CommandLineParser.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

class CLParser {
public:
	bool addSafe(CLArgBase *arg); // Add a new option to the options vector, avoid duplicates with warning

	CLParser& add(CLArgBase *arg); // Avoid duplicates but do not warn, and can be chained as in builder

	bool parse(const std::string &line) const; // Parse the line and fill the options

	std::vector<CLArgBase*> getOptions() const {
		return options;
	}
	;

private:
	bool check() const; // Check that the vector of options is coherent and the options are well defined

	std::vector<CLArgBase*> options;  // Contains the user defined options for the running program
};

bool CLParser::addSafe(CLArgBase *arg) {
	bool notfound { !pcontains<CLArgBase>(arg, options) };
	if (notfound) { // Avoid duplicates
		options.push_back(arg);
	}
	return notfound;
}

CLParser& CLParser::add(CLArgBase *arg) {
	addSafe(arg);
	return *this;
}

bool CLParser::parse(const std::string &line) const {
	if (!check())
		return false; // Check that the options are well defined, etc.

	std::vector<std::string> tokens = split(line, ' ');

	// 1. if -h or --help then show usage() and exit
	bool help = contains<std::string>("-h", tokens);
	if (help) {
		usage(options);
		return true;
	}
	// 2. parse the line and fill the parser
	auto lambdaOpt = [](const CLArgBase *a) {
		return a->getOption();
	};
	auto lambdaLongOpt = [](const CLArgBase *a) {
		return a->getLongOption();
	};
	for (CLArgBase *arg : options) {
		if (arg->getOption() == "-h")
			continue; // No need to process help since line has already been checked against it
		auto loc = locate(arg->getOption(), tokens);
		if (loc == tokens.cend()) {
			loc = locate(arg->getLongOption(), tokens);
		}
		if (loc == tokens.cend()) { // Option not found
			if (arg->isMandatory()) { // If this option was mandatory, exit with error code
				std::cout << "Mandatory option [" << arg->getOption() << " | " << arg->getLongOption()
						<< "] not found!\n";
				return false;
			}
		} else { // Option found
			if (arg->getType() == CLTYPE::NONE) { // If no value for this option, then just set it present
				arg->setPresent();
				continue;
			}
			int pos = loc - tokens.cbegin(); // A value must be defined next to this option keyword
			if (tokens.size() < pos) {
				std::cout << "Option [" << arg->getOption() << " | " << arg->getLongOption()
						<< "] has no value defined!\n";
				return false;
			}
			const std::string value = tokens[pos + 1];
			// Check that the value is not another option keyword
			auto loc2 = locate<std::string, CLArgBase*>(value, options, lambdaOpt);
			if (loc2 == options.cend()) {
				loc2 = locate<std::string, CLArgBase*>(value, options, lambdaLongOpt);
			}
			if (loc2 != options.cend()) {
				std::cout << "Option [" << arg->getOption() << " | " << arg->getLongOption()
						<< "] has no|forbidden value defined!\n";
				return false;
			}
			// Check that the value is of option expected type
			if (!fillOption(value, arg)) {
				std::cout << "Option [" << arg->getOption() << " | " << arg->getLongOption() << "] with value " << value
						<< " has no correct type defined!\n";
				std::cout << "Expected type : " << CLTYPEtoStr[arg->getType()] << "\n";
				return false;
			}
		}
	}

	return true;
}

bool CLParser::check() const {
	return true;
}

#endif //COMMAND_LINE_PARSER_C__CLPARSER_H
