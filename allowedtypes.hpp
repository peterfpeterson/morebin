#ifndef _ALLOWEDTYPES_HPP
#define _ALLOWEDTYPES_HPP

#include <iostream>
#include <string>
#include <vector>

namespace allowed {

class AllowedTypes {

public:
  void append(const std::string & name);
  bool has(const std::string & descr) const;
  bool empty() const;

  friend std::ostream& operator<<(std::ostream &os, const AllowedTypes & thing);

private:
  std::vector<std::string> allowed;
};

}

#endif // _ALLOWEDTYPES_HPP
