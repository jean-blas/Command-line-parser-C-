//
// Created by jimbert on 2/3/20.
//

#ifndef COMMAND_LINE_PARSER_C__CLPARSER_H
#define COMMAND_LINE_PARSER_C__CLPARSER_H

#include "CommandLineParser.h"
#include <vector>
#include <algorithm>

class CLParser {
public:
  bool addSafe(const CLArgBase &arg); // Add a new option to the options vector, avoid duplicates with warning

  CLParser &add(const CLArgBase &arg); // Avoid duplicates but do not warn, and can be chained as in builder

  bool parse(const std::string &line) const; // Parse the line and fill the options

  std::vector<CLArgBase> getOptions() const {
    return options;
  };

private:
  bool check() const; // Check that the vector of options is coherent and the options are well defined

  std::vector<CLArgBase> options;
};

bool CLParser::addSafe(const CLArgBase &arg) {
  bool notfound{!contains<CLArgBase>(arg, options)};
  if (notfound) { // Avoid duplicates
    options.push_back(arg);
  }
  return notfound;
}

CLParser &CLParser::add(const CLArgBase &arg) {
  addSafe(arg);
  return *this;
}

bool CLParser::parse(const std::string &line) const {
  if (!check())
    return false; // Check that the options are well defined, etc.

  // 1. if -h or --help then show usage() and exit
  auto lambdaOpt = [](const CLArgBase &a) {
    return a.getOption();
  };
  auto lambdaLongOpt = [](const CLArgBase &a) {
    return a.getLongOption();
  };
  auto loc = locate<std::string, CLArgBase>("-h", options, lambdaOpt);
  if (loc == options.cend()) {
    loc = locate<std::string, CLArgBase>("--help", options, lambdaLongOpt);
  }
  if (loc != options.cend()) {
    usage(options);
    exit(EXIT_SUCCESS);
  }
  // 2. parse the line and fill the parser
  std::vector<std::string> tokens{split(line, ' ')};
  for (CLArgBase arg : options) {
    loc = locate<std::string, CLArgBase>(arg.getOption(), options, lambdaOpt);
    if (loc == options.cend()) {
      loc = locate<std::string, CLArgBase>("--help", options, lambdaLongOpt);
    }
    if (loc == options.cend()) { // Option not found
      if (arg.isMandatory()) { // If this option was mandatory, exit with error code
        std::cout << "Mandatory option [" << arg.getOption() << " | " << arg.getLongOption() << "] not found!\n";
        exit(EXIT_FAILURE);
      }
    } else { // Option found
      if (arg.getType() == CLTYPE::NONE) { // If no value for this option, then just set it present
        arg.setPresent();
        continue;
      }
      int pos = loc - options.cbegin(); // A value must be defined next to this option keyword
      if (tokens.size() <= pos) {
        std::cout << "Option [" << arg.getOption() << " | " << arg.getLongOption() << "] has no value defined!\n";
        exit(EXIT_FAILURE);
      }
      const std::string value{tokens[pos]};
      // Check that the value is not another option keyword
      auto loc2 = locate<std::string, CLArgBase>(value, options, lambdaOpt);
      if (loc2 == options.cend()) {
        loc2 = locate<std::string, CLArgBase>(value, options, lambdaLongOpt);
      }
      if (loc2 != options.cend()) {
        std::cout << "Option [" << arg.getOption() << " | " << arg.getLongOption() << "] has no value defined!\n";
        exit(EXIT_FAILURE);
      }
      // Check that the value is of option expected type
      if (!fillOption(value, &arg)) {
        std::cout << "Option [" << arg.getOption() << " | " << arg.getLongOption() << "] with value " << value << " has no correct type defined!\n";
        std::cout << "Expected type : " << CLTYPEtoStr[arg.getType()] << "\n";
        exit(EXIT_FAILURE);
      }
    }
  }

  return true;
}

bool CLParser::check() const {
  return true;
}

#endif //COMMAND_LINE_PARSER_C__CLPARSER_H
