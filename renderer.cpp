#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "renderer.hpp"

using std::cout;
using std::endl;
using std::runtime_error;
using std::size_t;
using std::string;
using std::stringstream;
using std::vector;

namespace render {

class AllowedTypes {
public:
  AllowedTypes() {
    this->allowed = new vector<string>();
    this->allowed->push_back("char");
    this->allowed->push_back("uint8");
    this->allowed->push_back("uint16");
    this->allowed->push_back("uint32");
    this->allowed->push_back("uint64");
    this->allowed->push_back("int8");
    this->allowed->push_back("int16");
    this->allowed->push_back("int32");
    this->allowed->push_back("int64");
    this->allowed->push_back("float32");
    this->allowed->push_back("float64");
  }

  ~AllowedTypes() {
    if (this->allowed != NULL)
      delete this->allowed;
    this->allowed = NULL;
  }

  bool has(const string & descr) const {
    vector<string>::const_iterator pos
                    = std::find(this->allowed->begin(), this->allowed->end(), descr);
    return (pos != this->allowed->end());
  }

  friend std::ostream& operator<<(std::ostream &os, const AllowedTypes & thing) {
    size_t size = thing.allowed->size();
    for (size_t i = 0; i < size; i++) {
      os << thing.allowed->at(i);
      if (i + 1 < size)
	os << ", ";
    }
    return os;
  }

private:
  vector<string> * allowed;
};



Renderer::Renderer()
{
  this->dataDescr = new vector<string>();
}

Renderer::~Renderer()
{
  if (this->dataDescr != NULL)
    delete this->dataDescr;
  this->dataDescr = NULL;
}

void Renderer::setDataDescr(const std::string & descr)
{
  AllowedTypes allowed;
  if (! allowed.has(descr)) {
    stringstream msg;
    msg << "Encountered unknown type \"" << descr << "\". Allowed types are: "
	<< allowed;
    throw runtime_error(msg.str());
  }
  this->dataDescr->clear();
  this->dataDescr->push_back(descr);
}

template <typename NumT>
void innerShowData(BinFile &file, size_t offset, size_t length)
{
  vector<NumT> data;
  file.read(data, length);
  if (!(data.empty())) {
    for (size_t i = 0; i < data.size(); i++)
      cout << data[i];
    cout << endl;
  }
}

/// Special version for strings
template <>
void innerShowData<char>(BinFile &file, size_t offset, size_t length)
{
  stringstream data;
  file.read(data, length);
  cout << data.str() << endl;
}

void Renderer::showData(BinFile &file, size_t offset, size_t length)
{
  cout << "Renderer.showData(file, " << offset << ", " << length << ")" << endl;
  file.seek(offset);

  if (this->dataDescr->size() != 1)
    throw runtime_error("Do not know how to deal with multi-type data");

  string descr = this->dataDescr->at(0);

  if (descr == "char")
    innerShowData<char>(file, offset, length);
  else if (descr == "uint8")
    innerShowData<uint8_t>(file, offset, length);
  else if (descr == "int8")
    innerShowData<int8_t>(file, offset, length);
  else if (descr == "uint16")
    innerShowData<uint16_t>(file, offset, length);
  else if (descr == "int16")
    innerShowData<int16_t>(file, offset, length);
  else if (descr == "uint32")
    innerShowData<uint32_t>(file, offset, length);
  else if (descr == "int32")
    innerShowData<int32_t>(file, offset, length);
  else if (descr == "uint64")
    innerShowData<uint64_t>(file, offset, length);
  else if (descr == "int64")
    innerShowData<int64_t>(file, offset, length);
  else if (descr == "float")
    innerShowData<float>(file, offset, length);
  else if (descr == "double")
    innerShowData<double>(file, offset, length);
  else
    throw runtime_error("The code should have never gotten to this place");
}

const std::string getKnownDataDescr()
{
  stringstream msg;
  AllowedTypes types;
  msg << types;
  return msg.str();
}

} // namespace render
