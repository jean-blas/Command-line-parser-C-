//
// Created by jimbert on 2/3/20.
//

#ifndef COMMAND_LINE_PARSER_C__CLUTILS_H
#define COMMAND_LINE_PARSER_C__CLUTILS_H

#include <string>
#include <vector>
#include <functional>
#include <array>

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
template<typename TField, typename TItem> auto locate(const TField o, const std::vector<TItem> &v, std::function<TField(TItem)> getField) {
  auto lambda = [&o, &getField](const TItem &b) {
    return getField(b) == o;
  };
  // locate first occurrence of item with o field in v
  auto location = find_if(v.cbegin(), v.cend(), lambda);
  return location;
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

bool to+

bool fillOption(const std::string &value, CLArgBase *arg) {
  switch (arg->getType()) {
    case CLTYPE::STRING : {
      //      CLArg<std::string> *sarg = dynamic_cast<CLArg<std::string> *>(arg);
      auto *parg = (CLArg<std::string> *) (arg);
      parg->setValue(value);
      break;
    }
    case CLTYPE::BOOL : {
      auto *parg = (CLArg<bool> *) (arg);
      parg->setValue(str(value));
      break;
    }
    case CLTYPE::FLOAT : {
      auto *parg = (CLArg<float> *) (arg);
      parg->setValue(static_cast<float>value);
      break;
    }
    case CLTYPE::INT : {
      auto *parg = (CLArg<float> *) (arg);
      parg->setValue(static_cast<float>value);
      break;
    }
    case CLTYPE::QUOTED : {
      auto *parg = (CLArg<float> *) (arg);
      parg->setValue(static_cast<float>value);
      break;
    }
      break;
    default:
      return false;
  }
}

#endif //COMMAND_LINE_PARSER_C__CLUTILS_H
