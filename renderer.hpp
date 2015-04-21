#ifndef _RENDERER_HPP
#define _RENDERER_HPP 1

#include <cstddef>
#include <string>
#include <vector>

#include "allowedtypes.hpp"
#include "bin_file.hpp"

class BinFile;

namespace render {

const std::string getKnownDataDescr();

class Renderer
{
public:
  Renderer();
  virtual ~Renderer();
  virtual void setDataDescr(const std::string &);
  virtual void showData(BinFile &, std::size_t offset=0, std::size_t length=0);
  void showLines(const bool showLines);
  bool showLines() const ;
  void numItemsPerLine(const std::size_t numItems);
  std::size_t numItemsPerLine();
  void quiet(const bool value);
  bool quiet();
private:
  template <typename NumT>
  void innerShowData(BinFile &file, size_t offset, size_t length);
  std::vector<std::string> * m_dataDescr;
  allowed::AllowedTypes types;
  bool m_showLines;
  bool m_quiet;
  std::size_t m_numItemsPerLine;
};

} // namespace render

#endif
