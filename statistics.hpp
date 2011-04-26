#ifndef _STATISTICS_HPP
#define _STATISTICS_HPP 1

#include <iostream>
#include <vector>

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
  NumT total;
  NumT min;
  NumT max;
  std::size_t number;
};

#endif
