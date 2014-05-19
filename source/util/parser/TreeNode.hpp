#ifndef __TREENODE_HPP
#define __TREENODE_HPP

#include <string>
#include <vector>
#include "TreeNodeType.hpp"

namespace glPortal {
  namespace util{
    namespace parser{
    class TreeNode{
    private:
      TreeNodeType type;
      TreeNode *parent;
      std::string content;
      std::vector<TreeNode*> children;
    public:
      TreeNode(TreeNodeType type, std::string content);
      std::string getContent();
      TreeNodeType getType();
      void setParent(TreeNode *node);      
      TreeNode *getParent();      
      bool hasParent();
      std::vector<TreeNode *> getChildren();
      void addChild(TreeNode *child);      
      bool isLeaf();
    };
    }
  }
}

#endif