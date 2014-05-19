#ifndef __MAPFILEPARSER2_HPP
#define __MAPFILEPARSER2_HPP

#include <vector>
#include <string>
#include <cstring>
#include <cstdlib> 
#include <map>
#include "../../GameMap.hpp"
#include "../../util/parser/TreeNode.hpp"

using namespace glPortal::engine::object;
using namespace glPortal::util::parser;

namespace glPortal {
  namespace map{
    namespace parser{
    class MapFileParser2{
    private:
    public:
      GameMap getMapFromFile(std::string filename);
    };
    }
  }
}

#endif