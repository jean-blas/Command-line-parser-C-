//
// Created by Jean-Blas IMBERT on 2020-01-31.
//

#include "CommandLineParser.h"
#include "CLUtils.h"
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

    CLArg<string> clTest{"t", "test", CLTYPE::STRING, false, "Option without - nor --"};
    CLArg<string> clHelp{"-h", "--help", CLTYPE::NONE, true, "Display the available program options"};
    CLArg<bool> clBoolF{"-b", "", CLTYPE::BOOL, false, "No long option"};
    CLArg<bool> clBoolT{"", "--btrue", CLTYPE::BOOL, true, "No short option"};
    CLArg<float> clFloat{"-f", "--no_doc", CLTYPE::FLOAT, false};
    CLArg<int> clInt{"-iI", "--an_int", CLTYPE::INT, true, "short with 2 chars"};
    CLArg<string> clQuoted{"+q", "++qstr", CLTYPE::QUOTED, true, "Option with + and ++"};

    CLArg<string> clBuild = CLArg<string>::Builder{}.setCltype(CLTYPE::STRING).setOpt("-a")
            .setLongOpt("--aaa").isMandatory().setDoc("Documentation of option a")
            .build();

    vector<CLArgBase> v{clTest, clHelp, clBoolF, clBoolT, clFloat, clInt, clQuoted, clBuild};
    cout << usage(v);

    // TODO : parse command line into an array of CLArg

    // TODO : check options coherence and uniqueness

    return EXIT_SUCCESS;
}


