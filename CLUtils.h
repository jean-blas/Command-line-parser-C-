//
// Created by jimbert on 2/3/20.
//

#ifndef COMMAND_LINE_PARSER_C__CLUTILS_H
#define COMMAND_LINE_PARSER_C__CLUTILS_H

#include <string>
#include <vector>
#include <functional>
#include <array>
#include <iomanip>

// Split the string "s" with quotes inside and return the tokens into a vector
std::vector<std::string> split(const std::string &s) {
	std::istringstream iss(s);
	std::string item;
	std::vector<std::string> elems;
	while (iss >> std::quoted(item)) {
		elems.push_back(move(item));
	}
	return elems;
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

// Check if the given vector of pointers contains the given arg
template<typename T> bool pcontains(const T *arg, const std::vector<T*> &v) {
	auto it = std::find_if(v.cbegin(), v.cend(), [arg](T *t) {
		return *arg == *t;
	});
	return it != v.cend();
}

// Check if the given vector contains the given arg
template<typename T> bool contains(const T &arg, const std::vector<T> &v) {
	auto it = std::find(v.cbegin(), v.cend(), arg);
	return it != v.cend();
}

/* Locate the first occurrence of item with field defined by getField with value o
 @param o : TField value to target
 @param v : vector of items that have a field of type TField
 @param getField : function that takes an item from v and return its field of type TField

 Typically here, TItem = CLArgBase and TField = std::string
 */
template<typename TField, typename TItem> auto locate(const TField o, const std::vector<TItem> &v,
		std::function<TField(TItem)> getField) {
	auto lambda = [&o, &getField](const TItem &b) {
		return getField(b) == o;
	};
	// locate first occurrence of item with o field in v
	auto location = find_if(v.cbegin(), v.cend(), lambda);
	return location;
}

/* Locate the first occurrence of item with field defined by getField with value o
 @param o : T value to target
 @param v : vector of items that have a field of type T

 Typically here, T = std::string
 */
template<typename T> auto locate(const T &o, const std::vector<T> &v) {
	// locate first occurrence of o in v
	return std::find(v.cbegin(), v.cend(), o);
}

/* Locate the first occurrence of item with field defined by getField with values in iterators
 @param first, last : iterators of type TField to target
 @param v : vector of items that have a field of type TField
 @param getField : function that takes an item from v and return its field of type TField

 Typically here, TItem = CLArgBase, TField = std::string, first = vector.cbegin() and last = vector.cend()
 */
template<typename TField, typename TItem, typename ITER>
auto locateOR(ITER first, const ITER last, const std::vector<TItem> &v, std::function<TField(TItem)> getField) {
	auto lambda = [&first, &last, &getField](const TItem &b) {
		for (; first != last; ++first)
			if (getField(b) == *first)
				return true;
		return false;
	};
	// locate first occurrence of item with o field in v
	auto location = find_if(v.cbegin(), v.cend(), lambda);
	return location;
}

// Fill a CLArg value based on its type with the given parameter
bool fillOption(const std::string &value, CLArgBase *arg) {
	bool res = true;
	switch (arg->getType()) {
	case CLTYPE::STRING: {
		auto *parg = (CLArg<std::string>*) (arg);
		parg->setValue(value);
		break;
	}
	case CLTYPE::BOOL: {
		auto *parg = (CLArg<bool>*) (arg);
		if (value == "false")
			parg->setValue(false);
		else if (value == "true")
			parg->setValue(true);
		else
			res = false;
		break;
	}
	case CLTYPE::FLOAT: {
		auto *parg = (CLArg<float>*) (arg);
		try {
			parg->setValue(std::stof(value));
		} catch (...) {
			res = false;
		}
		break;
	}
	case CLTYPE::INT: {
		auto *parg = (CLArg<int>*) (arg);
		try {
			parg->setValue(std::stoi(value));
		} catch (...) {
			res = false;
		}
		break;
	}
	case CLTYPE::QUOTED: {
		auto *parg = (CLArg<std::string>*) (arg);
		std::stringstream ss;
		ss << std::quoted(value);
		parg->setValue(ss.str());
		break;
	}
	default:
		res = false;
	}
	return res;
}

#endif //COMMAND_LINE_PARSER_C__CLUTILS_H
