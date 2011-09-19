#include <algorithm>
#include "allowedtypes.hpp"

using std::size_t;
using std::string;
using std::vector;

namespace allowed {

AllowedTypes::AllowedTypes() {
  this->allowed = new std::vector<std::string>();
}

AllowedTypes::~AllowedTypes() {
  if (this->allowed != NULL)
    delete this->allowed;
  this->allowed = NULL;
}

AllowedTypes & AllowedTypes::operator=(const AllowedTypes &rhs) {
  if (!rhs.allowed->empty())
  {
    for (vector<string>::const_iterator it = rhs.allowed->begin();
	 it != rhs.allowed->end(); ++it)
    {
      this->allowed->push_back(*it);
    }
  }

  return *this;
}

void AllowedTypes::append(const std::string & name) {
  if (!name.empty())
    this->allowed->push_back(name);
}

bool AllowedTypes::has(const std::string & descr) const {
  std::vector<std::string>::const_iterator pos
    = std::find(this->allowed->begin(), this->allowed->end(), descr);
  return (pos != this->allowed->end());
}

bool AllowedTypes::empty() const {
  return this->allowed->empty();
}

std::ostream& operator<<(std::ostream &os, const AllowedTypes & thing) {
  if (thing.empty())
    return os;
  size_t size = thing.allowed->size();
  for (size_t i = 0; i < size; i++) {
    os << thing.allowed->at(i);
    if (i + 1 < size)
      os << ", ";
  }
  return os;
}

} // namespace allowed
