#ifndef PATH_TREE_H_
#define PATH_TREE_H_

#include <vector>

#include "util/common.h"

namespace mengde {
namespace core {

class PathTreeNode {
 public:
  PathTreeNode(Vec2D, PathTreeNode* = NULL);
  PathTreeNode* GetParent() { return parent_; }
  Vec2D GetData() { return data_; }
  bool IsRoot() { return parent_ == NULL; }

 private:
  Vec2D data_;
  PathTreeNode* parent_;
};


// PathTree is a tree data structure for saving unit's path
//
// Each PathTree node has a pointer to its parent but no children list.
// PathTree class has a node list that contains the entire nodes
// which is used for iterating all nodes and freeing when destruction.
// This is a useful data structure for saving path
// from Dijkstra's shortest path algorithm.

class PathTree {
 public:
  PathTree(Vec2D);
  ~PathTree();
  PathTreeNode* Adopt(Vec2D, PathTreeNode*);
  std::vector<Vec2D> GetNodeList();
  PathTreeNode* GetRoot() { return root_; }
  std::vector<Vec2D> GetPathToRoot(PathTreeNode*);
  std::vector<Vec2D> GetPathToRoot(Vec2D);
  PathTreeNode* FindNode(Vec2D);
  bool IsNodeExist(Vec2D v) { return FindNode(v) != NULL; }

 private:
  PathTreeNode* root_;
  std::vector<PathTreeNode*> node_list_;
};

} // namespace core
} // namespace mengde

#endif
