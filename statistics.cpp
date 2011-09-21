#include <limits>
#include <stdint.h>
#include "prenexustypes.hpp"
#include "statistics.hpp"
#include "string_util.hpp"

using std::ostream;
using std::string;
using std::stringstream;

// ------------------------------ StringStatistics
StringStatistics::StringStatistics()
{
  this->number = static_cast<std::size_t>(0);
}

StringStatistics::~StringStatistics()
{
}

void StringStatistics::parseData(std::stringstream &data)
{
  this->number += data.str().size(); // TODO there is a faster way
}

ostream& operator<<(ostream &os, const StringStatistics & thing)
{
  os << "SIZE: " << thing.number << " characters";
  return os;
}

// ------------------------------ PrenexusStatistics<ComplexT>


// ------------------------------ Statistics<NumT>

namespace { // anonymous namespace to hide from others

static const string MIN("MIN: ");
static const string MAX(" MAX: ");

template <typename NumT>
void getMinMax(NumT &min, NumT &max)
{
  min = std::numeric_limits<NumT>::max();
  max = std::numeric_limits<NumT>::min();
}

template <>
void getMinMax<double>(double &min, double &max)
{
  min = std::numeric_limits<double>::max();
  max = -1. * std::numeric_limits<double>::max();
}

template <>
void getMinMax<float>(float &min, float &max)
{
  min = std::numeric_limits<float>::max();
  max = -1. * std::numeric_limits<float>::max();
}

}

template <typename NumT>
Statistics<NumT>::Statistics()
{
  this->total = static_cast<NumT>(0.);
  this->number = static_cast<std::size_t>(0);
  getMinMax(this->min, this->max);
}

template <typename NumT>
Statistics<NumT>::~Statistics()
{
}

template <typename NumT>
void Statistics<NumT>::parseData(std::vector<NumT> & data)
{
  size_t size = data.size();
  this->number += size;

  NumT item;
  for (size_t i = 0; i < size; i++) {
    item = data[i];
    if (item < this->min)
      this->min = item;
    if (item > this->max)
      this->max = item;
    this->total += static_cast<double>(item);
  }
}

template <typename NumT>
ostream& operator<<(ostream &os, const Statistics<NumT>& thing)
{
  if (thing.number > 0)
    os << "MEAN: " << (thing.total / static_cast<double>(thing.number)) << " ";

  os << MIN << toStr(thing.min) << MAX << toStr(thing.max) << "\n";
  os << "SIZE: " << (thing.number*sizeof(thing.min)) << " bytes = "
     << thing.number << " x " << sizeof(thing.min) << " bytes";
  return os;
}

// concrete instantiations to make the compiler happy
template class Statistics<uint8_t>;
template class Statistics<int8_t>;
template class Statistics<uint16_t>;
template class Statistics<int16_t>;
template class Statistics<uint32_t>;
template class Statistics<int32_t>;
template class Statistics<uint64_t>;
template class Statistics<int64_t>;
template class Statistics<float>;
template class Statistics<double>;

template std::ostream& operator<<(std::ostream &, const Statistics<uint8_t> &);
template std::ostream& operator<<(std::ostream &, const Statistics<int8_t> &);
template std::ostream& operator<<(std::ostream &, const Statistics<uint16_t> &);
template std::ostream& operator<<(std::ostream &, const Statistics<int16_t> &);
template std::ostream& operator<<(std::ostream &, const Statistics<uint32_t> &);
template std::ostream& operator<<(std::ostream &, const Statistics<int32_t> &);
template std::ostream& operator<<(std::ostream &, const Statistics<uint64_t> &);
template std::ostream& operator<<(std::ostream &, const Statistics<int64_t> &);
template std::ostream& operator<<(std::ostream &, const Statistics<float> &);
template std::ostream& operator<<(std::ostream &, const Statistics<double> &);
