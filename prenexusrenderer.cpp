#include <iostream>
#include <stdexcept>
#include <sstream>
#include "prenexusrenderer.hpp"

using allowed::AllowedTypes;
using std::ostream;
using std::runtime_error;
using std::size_t;
using std::string;
using std::stringstream;
using std::vector;

namespace prenexus {

namespace { // anonymous namespace

static const string EOL("\n");

AllowedTypes getTypes() {
  AllowedTypes types;
  types.append("event");
  types.append("pulseid");
  types.append("oldpulseid");
  return types;
}

} // anonymous namespace

const string getKnownDataDescr()
{
  stringstream msg;
  msg << getTypes();
  return msg.str();
}

PrenexusRenderer::PrenexusRenderer()
{
  this->types = getTypes();
}

PrenexusRenderer::~PrenexusRenderer()
{
}

void PrenexusRenderer::setDataDescr(const string &descr)
{
  if (! this->types.has(descr)) {
    std::stringstream msg;
    msg << "Encountered unknown type \"" << descr << "\". Allowed types are: "
	<< this->types;
    throw runtime_error(msg.str());
  }
  this->m_dataDescr = descr;
}

void printValue(ostream & os, const DasEvent & value)
{
  os << value.tof << " \t" << value.pid;
}

void printValue(ostream & os, const Pulse & value)
{
  os << value.seconds << " \t" << value.nanoseconds << " \t" << value.event_index << " \t" << value.pCurrent;
}

void printValue(ostream & os, const OldPulse & value)
{
  os << value.seconds << " \t" << value.nanoseconds << " \t" << value.event_index;
}

template <typename ComplexT>
void PrenexusRenderer::innerShowData(BinFile &file, size_t offset, size_t length)
{
  // cache whether or not to show the data
  bool showLines = this->showLines();

  // this is used for printing line numbers
  size_t myOffset = 0;
  if ((offset % sizeof(ComplexT)) == 0)
    myOffset = offset / sizeof(ComplexT);

  // parse the file and print
  vector<ComplexT> data;
  file.read(data, length);
  if (!(data.empty())) {
    for (size_t i = 0; i < data.size(); i++)
    {
      if (showLines)
	std::cout << (myOffset + i + 1) << " ";
      printValue(std::cout,  data[i]);
      std::cout << EOL;
    }
  }
}

void PrenexusRenderer::showData(BinFile & file, size_t offset, size_t length)
{
  file.seek(offset);

  if (this->m_dataDescr.empty())
    throw runtime_error("Have an empty data description");

  if (this->m_dataDescr == "event")
    this->innerShowData<DasEvent>(file, offset, length);
  else if (this->m_dataDescr == "pulseid")
    this->innerShowData<Pulse>(file, offset, length);
  else if (this->m_dataDescr == "oldpulseid")
    this->innerShowData<OldPulse>(file, offset, length);
  else
    throw runtime_error("The code should have never gotten to this place");
  
}

} // namespace prenexus
