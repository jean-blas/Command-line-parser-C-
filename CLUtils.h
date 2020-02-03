//
// Created by jimbert on 2/3/20.
//

#ifndef COMMAND_LINE_PARSER_C__CLUTILS_H
#define COMMAND_LINE_PARSER_C__CLUTILS_H

#include <string>
#include <vector>

// Split the string "s" at the "delim" bounds and return the tokens into a vector
std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> elems;
  while (getline(ss, item, delim)) {
    elems.push_back(move(item));
  }
  return elems;
}

#endif //COMMAND_LINE_PARSER_C__CLUTILS_H
