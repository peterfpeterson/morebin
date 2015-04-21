#ifndef _BIN_FILE_HPP
#define _BIN_FILE_HPP 1

#include <boost/shared_ptr.hpp>
#include <stddef.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class BinFile
{
public:
  BinFile(const std::string &filename);
  virtual ~BinFile();
  // intentionally no copy constructor or assignment operator

  std::size_t size_in_bytes();

  /// The number of data items that must be in the file for the supplied data size.
  std::size_t num_items(const std::size_t data_size, const std::size_t items = 0);

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

#endif
