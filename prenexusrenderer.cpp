#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <ctime>
//include <time.h>
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

static const size_t BLOCK_SIZE = 1024;

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

/// Max allowed nanoseconds in the time; 2^62-1
static int64_t MAX_NANOSECONDS = 4611686018427387903LL;

/// Max allowed seconds in the time
static int64_t MAX_SECONDS      = 4611686017LL;

/// Min allowed nanoseconds in the time; -2^62+1
static int64_t MIN_NANOSECONDS  = -4611686018427387903LL;

/// Min allowed seconds in the time
static int64_t MIN_SECONDS      = -4611686017LL;

/// The epoch for GPS times.
static const boost::posix_time::ptime GPS_EPOCH(boost::gregorian::date(1990, 1, 1));

string toStr(const uint32_t seconds, const uint32_t nanoseconds)
{
  // turn it into a ptime
  boost::posix_time::ptime ptime_version
    = GPS_EPOCH + boost::posix_time::time_duration(0,0,seconds, nanoseconds);

  return to_iso_extended_string(ptime_version);
}

void printValue(ostream & os, const DasEvent & value)
{
  os << value.tof << " \t" << value.pid;
}

void printValue(ostream & os, const Pulse & value)
{
  os << toStr(value.seconds, value.nanoseconds) << " \t" 
     << value.event_index << " \t" << value.pCurrent;
}

void printValue(ostream & os, const OldPulse & value)
{
  os << toStr(value.seconds, value.nanoseconds) << " \t" << value.event_index;
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

  // set up to do a streaming read
  size_t size = file.num_items(sizeof(ComplexT));
  size_t num_read = BLOCK_SIZE;
  if (size < num_read)
      num_read = size;

  // parse the file and print
  size_t pos = 0;
  vector<ComplexT> data;
  data.reserve(num_read);
  while (pos < size) {
    file.read(data, num_read);
    if (!(data.empty())) {
      for (size_t i = 0; i < num_read; i++)
      {
        if (showLines)
          std::cout << (myOffset + i + 1) << " \t";
        printValue(std::cout,  data[i]);
        std::cout << EOL;
      }
    }

    // increment for the next time through the loop
    myOffset += num_read;
    pos += num_read;
    if (pos + num_read > size)
      num_read = size - pos;
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
