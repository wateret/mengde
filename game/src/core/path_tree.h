#ifndef MENGDE_CORE_PATH_TREE_H_
#define MENGDE_CORE_PATH_TREE_H_

#include <vector>

#include "util/common.h"

namespace mengde {
namespace core {

class PathNode {
 public:
  PathNode(Vec2D, PathNode* = NULL);
  PathNode* GetParent() { return parent_; }
  Vec2D     GetData() { return data_; }
  bool      IsRoot() { return parent_ == NULL; }

 private:
  Vec2D     data_;
  PathNode* parent_;
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
  PathNode*          Adopt(Vec2D, PathNode*);
  std::vector<Vec2D> GetNodeList();
  PathNode*          GetRoot() { return root_; }
  std::vector<Vec2D> GetPathToRoot(PathNode*);
  std::vector<Vec2D> GetPathToRoot(Vec2D);
  PathNode*          FindNode(Vec2D);
  bool               IsNodeExist(Vec2D v) { return FindNode(v) != NULL; }

 private:
  PathNode*              root_;
  std::vector<PathNode*> node_list_;
};

}  // namespace core
}  // namespace mengde

#endif // MENGDE_CORE_PATH_TREE_H_
