#include "allowedtypes.hpp"

#include "boost/algorithm/string.hpp"

#include <algorithm>
#include <cstddef>

using std::size_t;
using std::string;
using std::vector;

namespace allowed {

void AllowedTypes::append(const std::string & name) {
  if (!name.empty())
    this->allowed.push_back(name);
}

bool AllowedTypes::has(const std::string & descr) const {
  std::vector<std::string>::const_iterator pos
    = std::find(this->allowed.begin(), this->allowed.end(), descr);
  return (pos != this->allowed.end());
}

bool AllowedTypes::empty() const {
  return this->allowed.empty();
}

std::ostream& operator<<(std::ostream &os, const AllowedTypes & thing) {
  if (thing.empty())
    return os;
  os << boost::algorithm::join(thing.allowed, ", ");
  return os;
}

} // namespace allowed
