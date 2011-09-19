#include "prenexusrenderer.hpp"

using allowed::AllowedTypes;
using std::string;
using std::vector;

namespace prenexus {

namespace { // anonymous namespace

static const string EOL("\n");

AllowedTypes getTypes() {
  AllowedTypes types;
  types.append("event");
  types.append("pulse");
  types.append("oldpulse");
  return types;
}

} // anonymous namespace

const string getKnownDataDescr()
{
  stringstream msg;
  msg << getTypes();
  return msg.str();
}

} // namespace prenexus
