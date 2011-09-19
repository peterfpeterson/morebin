#ifndef _BIN_FILE_HPP
#define _BIN_FILE_HPP 1

#include <boost/shared_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>

class BinFile
{
public:
  BinFile(const std::string &filename);
  virtual ~BinFile();
  // intentionally no copy constructor or assignment operator

  std::size_t size_in_bytes();

  /// Seek to a position in the file relative to the beginning.
  /// \param bytes The position in bytes.
  void seek(const std::size_t bytes);

  /// Turn on/off byteswapping
  void setByteSwap(const bool);

  /// Read a chunk of data from the current offset position
  /// \param items the number of elements to read. Zero means read all.
  template <typename NumT>
  void read(std::vector<NumT> &data, const std::size_t items = 0);
  void read(std::stringstream & data, const size_t items = 0);

private:
  template <typename NumT>
  void read_block(NumT *buffer, const std::size_t buffer_size);

  bool byteSwap;
  std::size_t size_bytes;
  std::ifstream *handle;
  std::string filename;
};

namespace prenexus { // need a single place to include for these definitions
#pragma pack(push, 4) //Make sure the structure is 8 bytes.
struct DasEvent
{
  /// Time of flight.
  uint32_t tof;
  /// Pixel identifier as published by the DAS/DAE/DAQ.
  uint32_t pid;
};
#pragma pack(pop)

#pragma pack(push, 4) //Make sure the structure is 16 bytes.
struct Pulse
{
  /// The number of nanoseconds since the seconds field. This is not necessarily less than one second.
  uint32_t nanoseconds;

  /// The number of seconds since January 1, 1990.
  uint32_t seconds;

  /// The index of the first event for this pulse.
  uint64_t event_index;

  /// The proton charge for the pulse.
  double pCurrent;
};
#pragma pack(pop)

#pragma pack(push, 4) //Make sure the structure is the correct number of bytes.
struct OldPulse
{
  /// The number of nanoseconds since the seconds field. This is not necessarily less than one second.
  uint32_t nanoseconds;

  /// The number of seconds since January 1, 1990.
  uint32_t seconds;

  /// The index of the first event for this pulse.
  uint64_t event_index;
};
#pragma pack(pop)
} // namespace prenexus

#endif
