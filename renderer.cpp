#include <stdint.h>
#include <iostream>
#include <stdexcept>

#include "allowedtypes.hpp"
#include "bin_file.hpp"
#include "renderer.hpp"
#include "statistics.hpp"
#include "string_util.hpp"

using allowed::AllowedTypes;
using std::cout;
using std::endl;
using std::runtime_error;
using std::size_t;
using std::string;
using std::stringstream;
using std::vector;

namespace render {

namespace { // anonymous namespace

static const string EOL("\n");

AllowedTypes getTypes()
{
  AllowedTypes types;
  types.append("char");
  types.append("uint8");
  types.append("uint16");
  types.append("uint32");
  types.append("uint64");
  types.append("int8");
  types.append("int16");
  types.append("int32");
  types.append("int64");
  types.append("float32");
  types.append("float64");

  return types;
}

} // anonymous namespace


Renderer::Renderer(): m_showLines(false), m_quiet(false), m_numItemsPerLine(1)
{
  this->m_dataDescr = new vector<string>();
  this->types = getTypes();
}

Renderer::~Renderer()
{
  if (this->m_dataDescr != NULL)
    delete this->m_dataDescr;
  this->m_dataDescr = NULL;
}

void Renderer::setDataDescr(const std::string & descr)
{
  if (! this->types.has(descr)) {
    stringstream msg;
    msg << "Encountered unknown type \"" << descr << "\". Allowed types are: "
	<< this->types;
    throw runtime_error(msg.str());
  }
  this->m_dataDescr->clear();
  this->m_dataDescr->push_back(descr);
}

void Renderer::showLines(const bool showLines)
{
  this->m_showLines = showLines;
}

bool Renderer::showLines() const
{
  return this->m_showLines;
}

void Renderer::numItemsPerLine(const std::size_t numItems)
{
  if (numItems > 0)
    m_numItemsPerLine = numItems;
  else
    throw std::runtime_error("Tried to set number of items per line to less than 1");
}

std::size_t Renderer::numItemsPerLine()
{
  return m_numItemsPerLine;
}

void Renderer::quiet(const bool value)
{
  m_quiet = value;
}

bool Renderer::quiet()
{
  return m_quiet;
}

template <typename NumT>
void Renderer::innerShowData(BinFile &file, size_t offset, size_t length)
{
  // this is used for printing line numbers
  size_t myOffset = 0;
  if ((offset % sizeof(NumT)) == 0)
    myOffset = offset / sizeof(NumT);

  Statistics<NumT> stats; // object for generating statistics
  vector<NumT> data;
  size_t totalItems = this->numItemsPerLine() - 1;
  size_t items = 0;
  file.read(data, length);
  if (!(data.empty())) {
    stats.parseData(data);
    if (!m_quiet)
    {
      for (size_t i = 0; i < data.size(); i++) {
        if (this->m_showLines)
          cout << (myOffset + i + 1) << " "; // start counting with one
        cout << toStr(data[i]);
        if (items < totalItems)
        {
          cout << "\t";
          items += 1;
        }
        else
        {
          cout << EOL;
          items = 0;
        }
      }
    }

  }
  cout << stats << endl;
}

/// Special version for strings
template <>
void Renderer::innerShowData<char>(BinFile &file, size_t offset, size_t length)
{
  StringStatistics stats;
  stringstream data;
  file.read(data, length);
  if (!m_quiet)
    cout << data.str();
  stats.parseData(data);
  cout << stats << endl;
}

void Renderer::showData(BinFile &file, size_t offset, size_t length)
{
  // TODO have debug mode for this print statement
  // cout << "Renderer.showData(file, " << offset << ", " << length << ")" << endl;
  file.seek(offset);

  if (this->m_dataDescr->size() != 1)
    throw runtime_error("Do not know how to deal with multi-type data");
  string descr = this->m_dataDescr->at(0);

  // TODO calculate information on the fly rather than reading in the whole file
  // TODO there was the ability to show integrated values
  // TODO ? there was the ability to filter out tof error events

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
  else if (descr == "float32" || descr == "float")
    innerShowData<float>(file, offset, length);
  else if (descr == "float64" || descr == "double")
    innerShowData<double>(file, offset, length);
  else
    throw runtime_error("The code should have never gotten to this place");
}

const std::string getKnownDataDescr()
{
  stringstream msg;
  msg << getTypes();
  return msg.str();
}

} // namespace render
