//
// Created by Jean-Blas IMBERT on 2020-01-31.
//

#include "CommandLineParser.h"
#include <string>
#include <iostream>

using namespace std;

int main() {

    cout << "Enter some options with their values :\n";
    string line;
    getline(cin, line);

    vector<string> tokens{split(line, ' ')};

    for (const auto &s : tokens)
        cout << s << endl;

    CLArg<string> clTest{'t', "test", CLTYPE::STRING, false, "Option pour tester le pgm"};
    CLArg<string> clHelp{'h', "help", CLTYPE::STRING, true, "Display the available program options"};
    CLArg<bool> clBoolF{'b', "bfalse", CLTYPE::BOOL, false , "A boolean optional false argument"};
    CLArg<bool> clBoolT{'b', "btrue", CLTYPE::BOOL, true, "A boolean mandatory true argument"};
    CLArg<float> clFloat{'f', "fpoint", CLTYPE::FLOAT, false, "An optional floating point argument"};
    CLArg<int> clInt{'i', "an_int", CLTYPE::INT, true, "A mandatory integer argument"};
    CLArg<string> clQuoted{'q', "qstr", CLTYPE::QUOTED, true, "A quoted string argument"};

    vector<CLArgBase> v{clTest, clHelp, clBoolF, clBoolT, clFloat, clInt, clQuoted};
    cout << usage(v);
    // TODO : parse command line into an array of CLArg 

    return EXIT_SUCCESS;
}


