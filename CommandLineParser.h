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

// Enumerate the possible types for options in the command line
enum class CLTYPE {
    INT, FLOAT, BOOL, STRING, QUOTED,
    NONE // NONE => used when there is no value after the option (e.g. -h)
};

// Used to map the enum into string for printing
std::map<CLTYPE, std::string> CLTYPEtoStr{
        {CLTYPE::INT,    "integer"},
        {CLTYPE::FLOAT,  "float"},
        {CLTYPE::BOOL,   "boolean"},
        {CLTYPE::QUOTED, "quoted_string"},
        {CLTYPE::STRING, "string"},
        {CLTYPE::NONE,   ""}};

// Define and represents a Command Line Argument (or option) without any value
class CLArgBase {
public:
    explicit CLArgBase(std::string opt = "", std::string longOpt = "", CLTYPE clt = CLTYPE::STRING,
                       bool ismandatory = false, std::string docum = "") :
            option(std::move(opt)), longOption(std::move(longOpt)), cltype(clt), mandatory(ismandatory),
            doc(std::move(docum)) {};

    std::string getOption() const { return option; }

    std::string getLongOption() const { return longOption; }

    CLTYPE getType() const { return cltype; }

    std::string getDoc() const { return doc; }

    bool isMandatory() const { return mandatory; }

private:
    std::string option;      // mandatory field (e.g. -t)
    std::string longOption;  // e.g. --test
    CLTYPE cltype;           // type of option
    std::string doc;         // documentation
    bool mandatory;          // is this option mandatory or optional?
};

// Add a value to the command line option
template<typename T>
class CLArg : public CLArgBase {
public:
    class Builder; // construct the CLArg using the Builder pattern

    explicit CLArg(const std::string &opt = "", const std::string &longOpt = "", CLTYPE clt = CLTYPE::STRING,
                   bool ismandatory = false, const std::string &docum = "") :
            CLArgBase(opt, longOpt, clt, ismandatory, docum) {};

    T getValue() const { return value; }

    void setValue(T t) { value = t; }

private:
    T value;
};

// Construct a CLArg using the Builder pattern
template<typename T>
class CLArg<T>::Builder {
public:
    Builder &setOpt(const std::string &s) {
        option = s;
        return *this;
    }

    Builder &setLongOpt(const std::string &s) {
        longOption = s;
        return *this;
    }

    Builder &setCltype(CLTYPE t) {
        cltype = t;
        return *this;
    }

    Builder &isMandatory() {
        mandatory = true;
        return *this;
    }

    Builder &setDoc(const std::string &s) {
        doc = s;
        return *this;
    }

    CLArg<T> build() const { return CLArg<T>{option, longOption, cltype, mandatory, doc}; }

private:
    std::string option{}; // e.g. -t
    std::string longOption{""}; // e.g. --toto
    CLTYPE cltype{CLTYPE::NONE};
    std::string doc{""}; // documentation string
    bool mandatory{false}; // is this option mandatory or optional?
};

bool compOptionSize(const CLArgBase &a, const CLArgBase &b) {
    return a.getOption().size() < b.getOption().size();
}

bool compLongOptionSize(const CLArgBase &a, const CLArgBase &b) {
    return a.getLongOption().size() < b.getLongOption().size();
}

bool compTypeSize(const CLArgBase &a, const CLArgBase &b) {
    return CLTYPEtoStr[a.getType()].size() < CLTYPEtoStr[b.getType()].size();
}

// Display the available command line arguments in a nice format
std::string usage(const std::vector<CLArgBase> &options) {
    auto maxSizeLongOption{max_element(options.cbegin(), options.cend(), compLongOptionSize)};
    auto maxSizeOption{max_element(options.cbegin(), options.cend(), compOptionSize)};
    auto maxSizeType{max_element(options.cbegin(), options.cend(), compTypeSize)};
    std::ostringstream os;
    for (const CLArgBase &o : options) { // Display the full command line with all options
        os << "[";
        if (!o.getOption().empty()) os << o.getOption();
        if (!o.getOption().empty() && !o.getLongOption().empty()) os << " | ";
        if (!o.getLongOption().empty()) os << o.getLongOption();
        os << "] ";
    }
    os << "\nwhere:\n";
    for (const CLArgBase &o : options) // Display each option with its details
        os << "\t" << std::setw(maxSizeOption->getOption().size() + 1) << std::left << o.getOption()
           << ", " << std::setw(maxSizeLongOption->getLongOption().size() + 1) << std::left << o.getLongOption()
           << (o.isMandatory() ? " [M] " : " [O] ")
           << std::setw(CLTYPEtoStr[maxSizeType->getType()].size() + 1) << std::left << CLTYPEtoStr[o.getType()]
           << o.getDoc() << std::endl;
    return os.str();
}

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

#endif //DIVERSCPP_COMMANDLINEPARSER_H
