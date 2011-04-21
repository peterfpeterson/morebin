#include <iostream>
#include <stdexcept>
#include "bin_file.hpp"
#include "byte_swap.h"

using std::ifstream;
using std::runtime_error;
using std::size_t;
using std::string;
using std::vector;

namespace {
  static const size_t BLOCK_SIZE = 1024;
}

BinFile::BinFile(const string &filename) : byteSwap(false)
{
  // this should never trip
  if (filename.empty())
    throw runtime_error("Cannot specify empty filename");
  this->filename = filename;

  // open the file
  this->handle = new ifstream(this->filename.c_str(), std::ios::binary);
  if (!(this->handle->is_open()))
    throw runtime_error("Failed opening file");

  // determine the file size in bytes
  this->handle->seekg(0,std::ios::end);
  this->size_bytes = this->handle->tellg();
  this->seek(0);

}

BinFile::~BinFile()
{
  if (this->handle != NULL)
    delete this->handle;
  this->handle = NULL;
}

size_t BinFile::size_in_bytes()
{
  return this->size_bytes;
}

void BinFile::seek(const size_t bytes)
{
  if (this->handle == NULL)
    throw runtime_error("File handle is null");
  this->handle->seekg(bytes, std::ios::beg);
}

void BinFile::setByteSwap(const bool swap)
{
  this->byteSwap = swap;
}

namespace { // anonymous namespace hides from other files
void byte_swap(int8_t &number){
  // do nothing
  (void)number;
}
void byte_swap(int16_t &number){
  swap_bytes<2>(&number);
}
void byte_swap(int32_t &number){
  swap_bytes<4>(&number);
}
void byte_swap(int64_t &number){
  swap_bytes<8>(&number);
}
void byte_swap(uint8_t &number){
  // do nothing
  (void)number;
}
void byte_swap(uint16_t &number){
  swap_bytes<2>(&number);
}
void byte_swap(uint32_t &number){
  swap_bytes<4>(&number);
}
void byte_swap(uint64_t &number){
  swap_bytes<8>(&number);
}
void byte_swap(float &number){
  swap_bytes<4>(&number);
}
void byte_swap(double &number){
  swap_bytes<8>(&number);
}
}

template <typename NumT>
void BinFile::read_block(NumT *buffer, const std::size_t buffer_size) {
  size_t data_size=sizeof(NumT);

  this->handle->read(reinterpret_cast<char *>(buffer),buffer_size*data_size);

  if(this->byteSwap) {
    for( size_t i=0 ; i<buffer_size ; ++i ) {
      byte_swap(buffer[i]);
    }
  }

  /* REMOVE
  for( size_t i=0 ; i<buffer_size ; ++i ) {
    {
      if(config.show_data)
        {
          print_data_item<NumT>(buffer,offset,i,config);
        }
      if(config.sum_block)
        {
          sum_data_item<NumT>(buffer,i,config,sum,counter);
        }
    }
  if(!config.integrate.empty())
    {
      sum_events(buffer, buffer_size, event_count);
    }
  */
}

template <typename NumT>
void BinFile::read(vector<NumT> & data, const size_t items)
{
  data.clear();

  // cache the size of the fundamental data item
  size_t data_size = sizeof(NumT);

  //  determine how many items to read
  size_t fileStartPos = this->handle->tellg();
  size_t size = (this->size_bytes - fileStartPos) / data_size;
  if ((items > 0) && (items < size))
    size = items;

  // set up the buffer for reading

  // how many items to read
  size_t num_read = BLOCK_SIZE;
  if (size < num_read)
    num_read = size;
  NumT * buffer = new NumT[num_read];

  // the current position in the file (in items)
  size_t pos = 0;

#ifdef DEBUG
  std::cout << "data_size: " << data_size << " bytes" << std::endl;
  std::cout << "fileStartPos: " << fileStartPos << " bytes" << std::endl;
  std::cout << "size: " << size << " items " << std::endl;
  std::cout << "num_read: " << num_read << " items" << std::endl;
#endif

  // read in the data using the calculated blocks. Appending to the
  // i/o vector along the way
  while (pos < size) {
#ifdef DEBUG
    std::cout << "pos: " << pos << " num_read: " << num_read
	      << " size: " << size << std::endl;
#endif
    read_block(buffer, num_read);
    data.insert(data.end(), buffer, buffer+num_read);

    // calculate the next chunk to read
    pos += num_read;
    if (pos + num_read > size)
      num_read = size - pos;
  }

  delete buffer;
}

// concrete instantiations to make the compiler happy
template void BinFile::read<uint8_t>(vector<uint8_t> & data, const size_t items);
template void BinFile::read<int8_t>(vector<int8_t> & data, const size_t items);
template void BinFile::read<uint16_t>(vector<uint16_t> & data, const size_t items);
template void BinFile::read<int16_t>(vector<int16_t> & data, const size_t items);
template void BinFile::read<uint32_t>(vector<uint32_t> & data, const size_t items);
template void BinFile::read<int32_t>(vector<int32_t> & data, const size_t items);
template void BinFile::read<uint64_t>(vector<uint64_t> & data, const size_t items);
template void BinFile::read<int64_t>(vector<int64_t> & data, const size_t items);
template void BinFile::read<float>(vector<float> & data, const size_t items);
template void BinFile::read<double>(vector<double> & data, const size_t items);
