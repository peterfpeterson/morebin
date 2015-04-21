#ifndef _PRENEXUSRENDERER_HPP
#define _PRENEXUSRENDERER_HPP 1

#include <stddef.h>
#include <sstream>
#include <string>
#include <vector>

#include "allowedtypes.hpp"
#include "bin_file.hpp"
#include "renderer.hpp"

class BinFile;

using std::string;
using std::stringstream;
using std::vector;

namespace prenexus {

const std::string getKnownDataDescr();

class PrenexusRenderer: public render::Renderer
{
public:
  PrenexusRenderer();
  virtual ~PrenexusRenderer();
  virtual void setDataDescr(const std::string &);
  virtual void showData(BinFile &, std::size_t offset=0, std::size_t length=0);
private:
  template <typename ComplexT>
  void innerShowData(BinFile &file, size_t offset, size_t length);
  allowed::AllowedTypes types;
  std::string m_dataDescr;
};

} // namespace prenexus

#endif // _PRENEXUSRENDERER_HPP
