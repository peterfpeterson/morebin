#ifndef __BIN_FILE_H
#define __BIN_FILE_H 1

#include <boost/shared_ptr.hpp>
#include <fstream>
#include <string>
#include <vector>

#ifdef HAVE_STDINT_H
#include <stdint.h>
#else
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
typedef long int                int64_t;
typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long int       uint64_t;
#endif

class BinFile
{
public:
  BinFile(const std::string &filename);
  virtual ~BinFile();
  BinFile(const BinFile&);

  std::size_t size_in_bytes();

  /// Seek to a position in the file relative to the beginning.
  /// \param bytes The position in bytes.
  void seek(const std::size_t bytes);

  /// Read a chunk of data from the current offset position
  /// \param items the number of elements to read. Zero means read all.
  template <typename NumT>
  void read(std::vector<NumT> &data, const std::size_t items = 0);

private:
  template <typename NumT>
  void read_block(NumT *buffer, const std::size_t buffer_size);

  bool byte_swap;
  std::string filename;
  std::ifstream *handle;
  std::size_t size_bytes;
};

#endif
