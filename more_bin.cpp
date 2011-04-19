#include <tclap/CmdLine.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "byte_swap.h"

#ifdef _STDINT_H
#define HAVE_STDINT_H
#endif

// use STDINT if possible, otherwise define the types here
#ifdef _STDINT_H
#define HAVE_STDINT_H
#endif

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

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::invalid_argument;
using namespace TCLAP;

typedef uint32_t event_t;

struct Config{
  bool byte_swap;
  bool show_data;
  bool show_line_num;
  bool multi_file;
  bool sum_data;
  bool sum_block;
  bool is_event;
  bool get_pix_range;
  vector<size_t> integrate;
};

static const size_t BLOCK_SIZE=1024;
static const event_t ERROR=0x80000000;

/**
 * \file NXtranslate/binary/BinaryRetriever.cpp
 */

void byte_swap(int8_t &number){
  // do nothing
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

//static void set_event(Config &config) {

//}

static string::size_type count_occur(const string &str, const string &ch){
  string::size_type count = 0;
  string::size_type index = 0;

  // infinite loop to make sure that the entire string is parsed.
  while(true)
    {
      index = str.find(ch,index+1);
      if(index==string::npos)
        {
          break;
        }
      count++;
    }
  return count;
}

static bool my_isnotdigit(char c){
  return !isdigit(c);
}

static int str_to_int(const string &str){
  if(str.substr(0,1)=="-")
    return -1*str_to_int(str.substr(1,str.size()));

  string::const_iterator it = str.begin();
  it = find_if(it,str.end(),my_isnotdigit);

  if(it!=str.end())
    throw invalid_argument("str_to_int(string) argument is not an integer");

  return atol(str.c_str());
}

extern vector<string> split(const string &source,const string &split)
{
  string::size_type number=count_occur(source,split);
  if(number==0)
    {
      vector<string> result;
      result.push_back(source);
      return result;
    }

  vector<string> result;
  string::size_type start=0;
  string::size_type stop=0;
  string inner;
  while(true)
    {
      stop=source.find(split,start);
      if(stop==string::npos)
        {
          result.push_back(source.substr(start));
          break;
        }
      else
        {
          result.push_back(source.substr(start,stop-start));
          start=stop+split.size();
        }
    }
  return result;
}

static vector<int> str_to_intArray(const string & array){
  vector<string> str_array = split(array, ",");
  vector<int> result(0);
  size_t length = str_array.size();
  for (size_t i = 0; i < length; ++i) {
    result.push_back(str_to_int(str_array[i]));
  }
  return result;
}

template <typename NumT>
NumT get_pixid(const NumT *buffer, const size_t index, const NumT def_val) {
  return def_val;
}

event_t get_pixid(const event_t *buffer, size_t index, const event_t def_val) {
  if((buffer[index+1] & ERROR) != ERROR){
    return buffer[index+1];
  } else {
    return def_val;
  }
}

template <typename NumT>
void sum_events(const NumT *buffer, const size_t buffer_size,
                vector<NumT> &event_count) {
  return;
}

void increment_event(const event_t pixid, vector<event_t> &event_count) {
  event_t default_count = static_cast<event_t>(0);
  while(pixid>event_count.size()){
    event_count.push_back(default_count);
  }
  event_count[pixid]++;
}

void sum_events(const event_t *buffer, const size_t buffer_size,
                vector<event_t> &event_count) {
  event_t pixid;
  event_t error_id = static_cast<event_t>(-1);
  for ( size_t i=0 ; i<buffer_size ; i+=2 ) {
    pixid = get_pixid(buffer,i, error_id);
    if(pixid!=error_id){
      increment_event(pixid,event_count);
    }
  }
}

template <typename NumT>
NumT find_max_pixid(const NumT max_pixid, NumT *buffer,
                                                    const size_t buffer_size) {
  NumT my_max = max_pixid;
  NumT pixid;
  for ( size_t i=0 ; i<buffer_size ; i+=2 ) {
    pixid = get_pixid(buffer,i, max_pixid);
    if(pixid > my_max) {
      my_max=pixid;
    }
  }
  return my_max;
}

template <typename NumT>
NumT find_min_pixid(const NumT min_pixid, NumT *buffer,
                                                    const size_t buffer_size) {
  NumT my_min = min_pixid;
  NumT pixid;
  for ( size_t i=0 ; i<buffer_size ; i+=2 ) {
    pixid = get_pixid(buffer,i, min_pixid);
    if(pixid < my_min) {
      my_min=pixid;
    }
  }
  return my_min;
}

template <typename NumT>
string thing_to_str(const NumT thing) {
  std::stringstream s;
  s << thing;
  return s.str();
}

template <typename NumT>
string thing_to_str(const vector<NumT> &thing) {
  std::stringstream s;
  size_t length = thing.size();
  for ( size_t i = 0 ; i < length ; ++i ){
    s << thing[i] << " ";
  }
  return s.str();
}

string pixid_str(const size_t pixid, const vector<size_t> &bounds) {
  size_t rank = bounds.size();
  if (rank<=0) {
    return "";
  }
  size_t max_index=1;
  for (size_t i = 0 ; i<rank ; ++i ){
    max_index*=bounds[i];
  }
  if(pixid>max_index) {
    std::stringstream s;
    s << "Pixel id outside of known bounds: " << pixid << ">" << max_index;
    throw std::runtime_error(s.str());
  }
  size_t my_id = pixid;
  vector<size_t> indices(bounds.size());
  for (size_t i = 0 ; i < rank-1 ; ++i ) {
    indices[i] = my_id / bounds[i+1];
    my_id = my_id - indices[i] * bounds[i+1];
  }
  indices[rank-1] = my_id;

  return thing_to_str(indices);
}

/**
 * This function parses a single file and prints its contents
 *
 * \param type a number of the correct type to interpret the file
 * as. This is necessary for the function templating to work.
 * \param filename the name of the file to parse
 */
template <typename NumT>
void read_data(const string &filename, const Config &config)
{
  // open the file
  std::ifstream file(filename.c_str(),std::ios::binary);
  if(!(file.is_open()))
    {
      throw std::runtime_error("Failed opening file");
    }

  bool integrate = !config.integrate.empty();

  // determine the file size
  file.seekg(0,std::ios::end);
  long size_bytes=file.tellg();
  size_t data_size=sizeof(NumT);
  size_t size=size_bytes/data_size;
  if(!integrate){
    cout << "SIZE=(BYTES/DATA_SIZE): " << size << "=(" << size_bytes
         << "/" << data_size << ")" << endl;
  }
  // confirm that the integration size is compatible (histogram mode only)
  vector<NumT> event_count(0);
  if (integrate && !config.is_event) {
    size_t integrate_size = 1;
    size_t integrate_length = config.integrate.size();
    for (size_t i=0 ; i<integrate_length ; ++i ){
      integrate_size*=config.integrate[i];
    }
    if (integrate_size != size) {
      throw invalid_argument("Integration size does not match observed file size");
    }
    //    event_count = new vector<NumT>(integrate_size);
  }
  std::cout << "COUNT:" << event_count.size() << std::endl;

  if(config.show_data || config.sum_data || integrate || config.get_pix_range)
    {
      // seek back to the file start
      file.seekg(0,std::ios::beg);

      // temporary value to hold individual numbers
      NumT number;

      // holder for finding the id ranges
      NumT min_id = static_cast<NumT>(10000);
      NumT max_id = static_cast<NumT>(0);

      // value to hold the data sum
      long sum = 0;
      long counter = 0;

      // determine the data buffer size
      size_t block_size;
      if(config.integrate.empty()){
        block_size = BLOCK_SIZE;
      }else{
        block_size = *(config.integrate.rbegin());
      }

      // create a data buffer
      NumT *buffer=new NumT[block_size];

      // read through the file
      size_t offset=0;
      size_t num_read=block_size;
      if(size<block_size)
        {
          num_read=size;
        }
      size_t num_indices = 1;
      if (integrate)
        {
          num_indices = config.integrate.size() - 1; // do not count TOF
        }
      vector<size_t> indices(num_indices);
      long pixel_sum = 0;
      while(offset<size){
        read_block(file,buffer,offset,num_read,config,pixel_sum,counter,event_count);
        sum+=pixel_sum;
        if(integrate && !config.is_event) {
          // print out information
          if(pixel_sum>static_cast<NumT>(0)){
            for( size_t i = 0; i<indices.size(); ++i){
              cout << indices[i] << " ";
            }
            cout << pixel_sum << endl;
          }
          // increment the counters
          if(indices.size()==1){
            indices[0]++;
          } else {
            for( int i = indices.size()-1; i>0; --i){
              indices[i]++;
              if(indices[i]>=config.integrate[i]) {
                indices[i]=0;
                indices[i-1]++;
              }
            }
          }
          /*
          cout << i << " " << j << " " << pixel_sum << endl;
          j++;
          if(j>=config.integrate[1]){
            j=0;
            i++;
          }
          */
          pixel_sum = 0;
        }
        if(config.get_pix_range){
          min_id = find_min_pixid(min_id, buffer, num_read);
          max_id = find_max_pixid(max_id, buffer, num_read);
        }
        offset += num_read;
        if(offset+block_size>size)
          {
            num_read=size-offset;
          }
      }
      delete buffer;

      if(config.sum_data)
        {
          cout << counter << " : ";
          cout << sum << endl;
        }

      if(config.get_pix_range) {
        std::cout << "ID RANGE: " << min_id << " -> " << max_id << std::endl;
      }
    }

  if (integrate && config.is_event) {
    size_t length = event_count.size();
    for ( size_t i = 0 ; i < length ; ++i ) {
      if (event_count[i]!=static_cast<NumT>(0)) {
        cout << pixid_str(i, config.integrate)
             << " " << event_count[i] << endl;
      }
    }
  }

  // close the file
  file.close();
}

template <typename NumT>
void print_data_item(const NumT *buffer, const size_t offset, const size_t index, const Config &config){
  if(config.is_event)
    {
      if( index%2==0 )
        {
          if(config.show_line_num)
            {
              cout << 1+(index+offset)/2 << ":";
            }
          cout.width(10);
          cout << buffer[index];
          cout.width(20);
          cout << buffer[index+1] << endl;
        }
    }
    else
      {
        if(config.show_line_num)
          {
            cout << (1+index+offset) << ":";
          }
        cout << buffer[index] << endl;
      }
}

void sum_data_item(const event_t *buffer, const size_t index, const Config &config, long &sum, long &counter){
  if(config.is_event)
    {
      if( index%2==0 )
        {
          if((buffer[index+1] & ERROR) != ERROR){
            sum++;
          }
          counter++;
        }
    }
  else
    {
      sum += static_cast<long>(buffer[index]);
      counter++;
    }
}

template <typename NumT>
void sum_data_item(const NumT *buffer, const size_t index, 
                   const Config &config, long &sum, long &counter){
  sum += static_cast<long>(buffer[index]);
  counter++;
}

template <typename NumT>
void read_block(std::ifstream &file,
                NumT *buffer, const size_t offset, const size_t buffer_size,
                const Config &config,
                long &sum, long &counter, vector<NumT> &event_count){

  size_t data_size=sizeof(NumT);

  file.seekg(offset*data_size,std::ios::beg);
  file.read(reinterpret_cast<char *>(buffer),buffer_size*data_size);

  if(config.byte_swap)
    {
      for( size_t i=0 ; i<buffer_size ; ++i )
        {
          byte_swap(buffer[i]);
        }
    }

  for( size_t i=0 ; i<buffer_size ; ++i )
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
}

/**
 * This function takes a list of files and parses them all.
 *
 * \param type a number of the correct type to interpret the file
 * as. This is necessary for the function templating to work.
 * \param files is the list of files to parse
 */
template <typename NumT>
void read_data(const vector<string> &files, const Config &config)
{
  for( int i=0 ; i<files.size() ; i++ )
    {
      if(config.multi_file)
        {
          cout << files[i] << ":";
          if(config.show_data)
            {
              cout << endl;
            }
        }
      read_data<NumT>(files[i],config);
    }
}

/**
 * The main entry point for the program.
 */
int main(int argc, char** argv)
{
  try
    {
      // set up a parser
      CmdLine cmd("Command line description message", ' ', "0.0alpha");

      // command line arguments for declaring the data type
      vector<string> allowed_types;
      allowed_types.push_back("uint8");
      allowed_types.push_back("uint16");
      allowed_types.push_back("uint32");
      allowed_types.push_back("uint64");
      allowed_types.push_back("int8");
      allowed_types.push_back("int16");
      allowed_types.push_back("int32");
      allowed_types.push_back("int64");
      allowed_types.push_back("float32");
      allowed_types.push_back("float64");
      ValuesConstraint<string> allowed_types_constraint(allowed_types);
      ValueArg<string> typeArg("t","type","Type to interpret the file as",
                               false, "uint32", &allowed_types_constraint, cmd);
      // the filename arguments
      UnlabeledMultiArg<string> filenameArg("filename",
                                            "Name of a file to be viewed",
                                            true,
                                            "filename", cmd);
      // the swap argument
      SwitchArg swapArg("s", "swap", "Byteswap the file", cmd);
      // show data
      SwitchArg showDataArg("o","showdata","Print the values in the file", cmd);
      // show the line numbers
      SwitchArg showLineNumArg("n","linenumber",
                      "Show the line numbers next values. Forces '--showdata'", cmd);
      
      // sum the data in the file
      SwitchArg sumDataArg("u", "sum","Sum the data in the file",cmd);

      // set this as event file
      SwitchArg eventDataArg("e","event","Interpret as an event file",cmd);
      // integrate values of 3D buffer
      ValueArg<string> integrateArg("","integrate",
                                  "Integrate the data as a multidimensional array along the fastest varying axis. This suppresses showdata argument.",
                                  false, "", "dimensions", cmd);

      // determine the maximum pixel id (linear)
      SwitchArg pixidRangeArg("", "pixrange",
                           "Determine the minimum and maximum pixel id. Forces event mode",
                            cmd);

      // parse the filename
      cmd.parse(argc,argv);

      // get the type
      string type=typeArg.getValue();

      // the list of files
      vector<string> files=filenameArg.getValue();
      if(files.empty())
        {
          std::cerr << "ERROR: failed to supply <filename>" << endl;
          cmd.getOutput()->usage(cmd);
          return -1;
        }

      // fill the config object
      struct Config config;
      config.byte_swap=swapArg.getValue();
      config.show_data=showDataArg.getValue();
      config.show_line_num=showLineNumArg.getValue();
      if(config.show_line_num)
        {
          config.show_data=true;
        }
      config.multi_file=(files.size()>1);
      config.sum_data=sumDataArg.getValue();
      config.sum_block=config.sum_data;
      if(eventDataArg.getValue()){
        config.is_event=true;
      }else{
        config.is_event=false;
      }
      if(integrateArg.getValue().size()>0){
        vector<int> int_array=str_to_intArray(integrateArg.getValue());
        config.integrate.assign(int_array.begin(), int_array.end());
        config.show_data=false;
        config.sum_block=true;
      }
      if(pixidRangeArg.getValue()){
        config.get_pix_range = true;
        config.is_event = true;
      }

      // print out the contents of the file
      if(config.is_event)
        {
          read_data<event_t>(files,config);
        }
      else if(type=="uint8")
        {
          read_data<uint8_t>(files,config);
        }
      else if(type=="uint16")
        {
          read_data<uint16_t>(files,config);
        }
      else if(type=="uint32")
        {
          read_data<uint32_t>(files,config);
        }
      else if(type=="uint64")
        {
          read_data<uint64_t>(files,config);
        }
      else if(type=="int8")
        {
          read_data<int8_t>(files,config);
        }
      else if(type=="int16")
        {
          read_data<int16_t>(files,config);
        }
      else if(type=="int32")
        {
          read_data<int32_t>(files,config);
        }
      else if(type=="int64")
        {
          read_data<int64_t>(files,config);
        }
      else if(type=="float32")
        {
          read_data<float>(files,config);
        }
      else if(type=="float64")
        {
          read_data<double>(files,config);
        }
      else
        {
          std::cerr << "ERROR: Do not understand type \"" << type
                    << "\"" << endl;
          return -1;
        }
    }
  catch(ArgException &e)
    {
      std::cerr << "PARSE ERROR:" << e.error() << " for arg " << e.argId()
                << endl;
    }
  catch(std::runtime_error &e)
    {
      std::cerr << "RUNTIME ERROR:" << e.what() << endl;
      return -1;
    }

  return 0;
}
