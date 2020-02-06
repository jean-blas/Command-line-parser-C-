//
// Created by Jean-Blas IMBERT on 2020-01-31.
//

#include "CommandLineParser.h"
#include "CLUtils.h"
#include "CLParser.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

int main() {

  cout << "Enter some options with their values :\n";
  string line;
  getline(cin, line);

  vector<string> tokens{split(line, ' ')};

  for (const auto &s : tokens)
    cout << s << endl;

  CLArg<string> clTest{"t", "test", CLTYPE::STRING, false, "Option without - nor --"};
  CLArg<string> clHelp{"-h", "--help", CLTYPE::NONE, false, "Display the available program options"};
  CLArg<bool> clBoolF{"-b", "", CLTYPE::BOOL, false, "No long option"};
//  CLArg<bool> clBoolT{"", "--btrue", CLTYPE::BOOL, true, "No short option"};
  CLArg<float> clFloat{"-f", "--no_doc", CLTYPE::FLOAT, false};
//  CLArg<int> clInt{"-iI", "--an_int", CLTYPE::INT, true, "short with 2 chars"};
//  CLArg<string> clQuoted{"+q", "++qstr", CLTYPE::QUOTED, true, "Option with + and ++"};
//  CLArg<string> clBuild =
//      CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setOpt("-a").setLongOpt("--aaa").isMandatory().setDoc("Documentation of option a").build();

  CLParser parser;
  parser.add(clTest)
		  .add(clHelp)
		  .add(clBoolF)
//		  .add(clBoolT)
		  .add(clFloat)
//		  .add(clInt)
//		  .add(clQuoted)
//		  .add(clBuild)
		  ;

  cout << usage(parser.getOptions());

  bool parsed{parser.parse(line)};
  if (!parsed) {
	  cout << "Unable to parse the command line!\nPlease correct...";
	  exit(EXIT_FAILURE);
  }

  for (CLArgBase cl : parser.getOptions()) {
	  cout << "Option " << cl.to_string() << " : " << endl;
  }

  // TODO : add option ENUM

  // TODO : check options coherence

  return EXIT_SUCCESS;
}

