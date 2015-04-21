#ifndef _STATISTICS_HPP
#define _STATISTICS_HPP 1

#include <stdint.h>
#include <iostream>
#include <sstream>
#include <vector>

class StringStatistics
{
friend std::ostream& operator<<(std::ostream &os,
				const StringStatistics & thing);
public:
  StringStatistics();
  virtual ~StringStatistics();

  void parseData(std::stringstream &data);

private:
  std::size_t number;
};

template <typename ComplexT>
class PrenexusStatistics
{
template <typename TYPE>
friend std::ostream& operator<<(std::ostream &os, const PrenexusStatistics<TYPE> &thing);

public:
  PrenexusStatistics();
  virtual ~PrenexusStatistics();

  void parseData(std::vector<ComplexT> & data);

private:
  std::size_t number;
};

template <typename NumT>
class Statistics
{
template <typename TYPE>
friend std::ostream& operator<<(std::ostream &os,
				const Statistics<TYPE> & thing);

public:
  Statistics();
  virtual ~Statistics();

  void parseData(std::vector<NumT> & data);

private:
  double total;
  NumT min;
  NumT max;
  std::size_t number;
};

#endif
