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

	// Define some options to be tested in this program
	CLArg<string> clTest { "t", "test", CLTYPE::STRING, false, "Option without - nor --" };
	CLArg<string> clHelp { "-h", "--help", CLTYPE::NONE, false, "Display the available program options" };
	CLArg<bool> clBoolF { "-b", "", CLTYPE::BOOL, false, "No long option" };
//  CLArg<bool> clBoolT{"", "--btrue", CLTYPE::BOOL, true, "No short option"};
	CLArg<float> clFloat { "-f", "--no_doc", CLTYPE::FLOAT, false };
//  CLArg<int> clInt{"-iI", "--an_int", CLTYPE::INT, true, "short with 2 chars"};
//  CLArg<string> clQuoted{"+q", "++qstr", CLTYPE::QUOTED, true, "Option with + and ++"};
//  CLArg<string> clBuild =
//      CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setOpt("-a").setLongOpt("--aaa").isMandatory().setDoc("Documentation of option a").build();

	CLParser parser; //1. Create a Parser with empty options

	//2. Fill the parser with the wanted options
	parser.add(&clTest).add(&clHelp).add(&clBoolF).add(&clFloat);
	//		  .add(&clBoolT).add(&clInt).add(&clQuoted).add(&clBuild)

	bool parsed { parser.parse(line) }; //3. Parse the command line according to the options and fill them
	if (!parsed) {
		cout << "Unable to parse the command line!\nPlease correct...";
		exit(EXIT_FAILURE);
	}

	//4. If (clTest.isPresent) { do_something ... }

	cout << usage(parser.getOptions()); // Example of usage()

	for (CLArgBase *cl : parser.getOptions()) {
		cout << "\n" << cl->to_string();
	}

	// TODO : add option ENUM

	// TODO : check options coherence

	return EXIT_SUCCESS;
}

