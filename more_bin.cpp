//include <tclap/CmdLine.h>
#include <algorithm>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "bin_file.hpp"
#include "prenexusrenderer.hpp"
#include "renderer.hpp"
#include "version.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::string;
using std::stringstream;
using std::vector;
namespace po = boost::program_options;

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
static const string DEFAULT_TYPE="char";

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
  it = std::find_if(it,str.end(),my_isnotdigit);

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
 * The main entry point for the program.
 */
int main(int argc, char** argv)
{
  // ---------- Declare the supported options.
  po::options_description generic_options("Generic options");
  generic_options.add_options()
    ("help,h", "Produce this help message.")
    ("version,v", "Print version string.")
    ;

  stringstream typesHelp;
  typesHelp << "Set the type of the data. Allowed values are: " << render::getKnownDataDescr() << ", " << prenexus::getKnownDataDescr();
  po::options_description config_options("Configuration");
  config_options.add_options()
    ("type,t", po::value<string>()->default_value(DEFAULT_TYPE), typesHelp.str().c_str())
    ("offset", po::value<size_t>()->default_value(0), "Skip to this position (in bytes) in the file.")
    ("length", po::value<size_t>()->default_value(0), "Number of items to read (NOT in bytes). Zero means read to end of file.")
    ("byteswap", "Perform byte swapping on the data")
    ("lines", "show line numbers")
    ;

  po::options_description hidden_options;
  hidden_options.add_options()
    ("filename", po::value< vector<string> >(), "input files")
    ;

  // all options available on the command line
  po::options_description cmdline_options;
  cmdline_options.add(generic_options).add(config_options).add(hidden_options);

  // all options visible to the user
  po::options_description visible_options("Allowed options");
  visible_options.add(generic_options).add(config_options);

  // ---------- parse the command line
  po::positional_options_description p;
  p.add("filename", -1);
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(cmdline_options)
	    .positional(p).run(), vm);
  po::notify(vm);

  // ---------- get everything out of the options
  // work with generic options
  if (vm.count("help")) {
    cout << "Usage: " << argv[0] << " [options] <filenames>" << endl;
    cout << endl;
    cout << visible_options << endl;
    return 1;
  }
  if (vm.count("version")) {
    cout << "morebin version " << VERSION << " built " << DATE << endl;
    return 0;
  }

  // config options
  size_t offset = 0;
  if (vm.count("offset"))
    offset = vm["offset"].as<size_t>();
  size_t length = 0;
  if (vm.count("length"))
    length = vm["length"].as<size_t>();
  bool byteswap = (vm.count("byteswap") > 0);
  string dataType = DEFAULT_TYPE;
  if (vm.count("type"))
    dataType = vm["type"].as<string>();
  bool showLines = (vm.count("lines") > 0);

  // hidden options
  vector<string> files;
  if (vm.count("filename"))
  {
    files = vm["filename"].as< vector<string> >();
  }
  else
  {
    cerr << "ERROR: failed to supply <filename>" << endl;
    //cmd.getOutput()->usage(cmd);
    return -1;
  }

  // ---------- push through the list of files
  try {
    bool multiFile = (files.size() > 1);
    render::Renderer renderer;
    renderer.setDataDescr(dataType);
    renderer.showLines(showLines);

    for (std::size_t i = 0; i < files.size(); i++) {
      BinFile file(files[i]);
      file.setByteSwap(byteswap);
      if (multiFile) {
	cout << "******************************" << endl;
	cout << "* " << files[i] << " size:" << file.size_in_bytes() << endl;
	cout << "******************************" << endl;
      }
      renderer.showData(file, offset, length);
    }
    
  } catch(std::runtime_error &e) {
    cerr << "RUNTIME ERROR:" << e.what() << endl;
    return -1;
  }

  return 0;
}
