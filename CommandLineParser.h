#include <utility>

//
// Created by Jean-Blas IMBERT on 2020-01-31.
//

#ifndef DIVERSCPP_COMMANDLINEPARSER_H
#define DIVERSCPP_COMMANDLINEPARSER_H

#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// Enumerate the possible types for options in the command line
enum class CLTYPE {
    INT, FLOAT, BOOL, STRING, QUOTED
};

// Used to map the enum into string for printing
std::map<CLTYPE, std::string> CLTYPEtoStr{
        {CLTYPE::INT,    "integer"},
        {CLTYPE::FLOAT,  "float"},
        {CLTYPE::BOOL,   "boolean"},
        {CLTYPE::QUOTED, "quoted_string"},
        {CLTYPE::STRING, "string"}};

// Define and represents a Command Line Argument (or option) without any value
class CLArgBase {
public:
    explicit CLArgBase(char opt = ' ', std::string longOpt = "", CLTYPE clt = CLTYPE::STRING, bool ismandatory = false, std::string docum = "") :
            option(opt), longOption(std::move(longOpt)), cltype(clt), mandatory(ismandatory), doc(std::move(docum)) {};

    char getOption() const { return option; }

    std::string getLongOption() const { return longOption; }

    CLTYPE getType() const { return cltype; }

    std::string getDoc() const { return doc; }

    bool isMandatory() const { return mandatory; }

private:
    char option; // e.g. -t
    std::string longOption; // e.g. --test
    CLTYPE cltype; // e.g. bool
    std::string doc; // documentation
    bool mandatory; // is this option mandatory or optional?
};

// Add a value to the command line option
template<typename T>
class CLArg : public CLArgBase {
public:
    explicit CLArg(char opt = ' ', const std::string& longOpt = "", CLTYPE clt = CLTYPE::STRING, bool ismandatory = false, std::string docum = "") :
            CLArgBase(opt, longOpt, clt, ismandatory, docum) {};

    T getValue() const { return value; }

    void setValue(T t) { value = t; }

private:
    T value; // e.g. false
};

bool compLongOptionSize(const CLArgBase& a, const CLArgBase& b) {
    return a.getLongOption().size() < b.getLongOption().size();
}

bool compTypeSize(const CLArgBase& a, const CLArgBase& b) {
    return CLTYPEtoStr[a.getType()].size() < CLTYPEtoStr[b.getType()].size();
}


// Display the available command line arguments in a nice format
string usage(const vector<CLArgBase>& options) {
    auto maxSizeLongOption{max_element(options.cbegin(), options.cend(), compLongOptionSize)};
    auto maxSizeType{max_element(options.cbegin(), options.cend(), compTypeSize)};
    ostringstream os;
    for (const CLArgBase& o : options)
        os << "-" << o.getOption()
           << ", --" << setw(maxSizeLongOption->getLongOption().size() + 1) << left << o.getLongOption()
           << (o.isMandatory() ? " [M] " : " [O] ")
           << setw(CLTYPEtoStr[maxSizeType->getType()].size() + 1) << left << CLTYPEtoStr[o.getType()]
           << o.getDoc() << endl;
    return os.str();
}

// Split the string "s" at the "delim" bounds and return the tokens into a vector
vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> elems;
    while (getline(ss, item, delim)) {
        elems.push_back(move(item));
    }
    return elems;
}

#endif //DIVERSCPP_COMMANDLINEPARSER_H
