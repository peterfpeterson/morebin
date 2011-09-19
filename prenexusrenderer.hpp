#ifndef _PRENEXUSRENDERER_HPP
#define _PRENEXUSRENDERER_HPP 1

#include <string>
#include <sstream>
#include <vector>
#include "allowedtypes.hpp"
#include "bin_file.hpp"
#include "renderer.hpp"

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
  void setDataDescr(const std::string &);
  void showData(BinFile &, std::size_t offset=0, std::size_t length=0);
  void showLines(const bool showLines);
private:
  template <typename NumT>
  void innerShowData(BinFile &file, size_t offset, size_t length);

  std::vector<std::string> * m_dataDescr;
  allowed::AllowedTypes types;
  bool m_showLines;
};

} // namespace prenexus

#endif // _PRENEXUSRENDERER_HPP
