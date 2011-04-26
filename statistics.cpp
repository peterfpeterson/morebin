#include <limits>
#include <stdint.h>
#include "statistics.hpp"

namespace { // anonymous namespace to hide from others

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
    this->total += item;
  }
}

template <typename NumT>
std::ostream& operator<<(std::ostream &os, const Statistics<NumT> & thing) {
  if (thing.number > 0) {
    double mean = static_cast<double>(thing.total);
    mean = mean / static_cast<double>(thing.number);
    os << "MEAN " << mean << " ";
  }

  os << "MIN " << thing.min << " MAX: " << thing.max
     << " TOTAL ELEMENTS: " << thing.number;
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
