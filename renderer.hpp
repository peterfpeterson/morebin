#ifndef _RENDERER_HPP
#define _RENDERER_HPP 1

#include <string>
#include <vector>
#include "bin_file.hpp"

namespace render {

const std::string getKnownDataDescr();

class Renderer
{
public:
  Renderer();
  virtual ~Renderer();
  void setDataDescr(const std::string &);
  void showData(BinFile &, std::size_t offset=0, std::size_t length=0);
private:
  std::vector<std::string> * dataDescr;
};

} // namespace render

#endif
