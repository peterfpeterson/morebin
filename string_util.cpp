#include "string_util.hpp"

#include <stdint.h>
#include <sstream>

using std::string;
using std::stringstream;

template <typename NumT>
const std::string toStr(const NumT number)
{
  stringstream result;
  result << number;
  return result.str();
}

template <>
const std::string toStr(const uint8_t number)
{
  stringstream result;
  result << static_cast<uint16_t>(number);
  return result.str();
}

template <>
const std::string toStr(const int8_t number)
{
  stringstream result;
  result << static_cast<int16_t>(number);
  return result.str();
}

//template const string toStr(const uint8_t number);
//template const string toStr(const int8_t number);
template const string toStr(const uint16_t number);
template const string toStr(const int16_t number);
template const string toStr(const uint32_t number);
template const string toStr(const int32_t number);
template const string toStr(const uint64_t number);
template const string toStr(const int64_t number);
template const string toStr(const float number);
template const string toStr(const double number);
