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

  CLParser &add(const CLArgBase &arg) { // Avoid duplicates but do not warn, and can be chained as in builder
    addSafe(arg);
    return *this;
  }

  bool parse(const std::string &line) const; // Parse the line and fill the options

  std::vector<CLArgBase> getOptions() const {
    return options;
  };

private:
  bool check() const; // Check that the vector of options is coherent and the options are well defined
  std::vector<CLArgBase> options;
};

bool CLParser::addSafe(const CLArgBase &arg) {
  if (std::find(options.cbegin(), options.cend(), arg) == options.cend()) { // Avoid duplicates
    options.push_back(arg);
    return true;
  }
  return false;
}

bool CLParser::parse(const std::string &line) const {
  if (!check())
    return false; // Check that the options are well defined, etc.

  // TODO parse the line and fill the options
  return true;
}

bool CLParser::check() const {
  return true;
}

#endif //COMMAND_LINE_PARSER_C__CLPARSER_H
