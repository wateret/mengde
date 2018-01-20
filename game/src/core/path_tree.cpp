#include "path_tree.h"

namespace mengde {
namespace core {

// PathNode

PathNode::PathNode(Vec2D data, PathNode* parent)
    : data_(data),
      parent_(parent) {
}

// PathTree

PathTree::PathTree(Vec2D root_data) {
  root_ = new PathNode(root_data);
  node_list_.push_back(root_);
}

PathTree::~PathTree()
{
  for (auto itr : node_list_) {
    delete itr;
  }
}

PathNode* PathTree::Adopt(Vec2D child_data, PathNode* parent_node) {
  PathNode* new_node = new PathNode(child_data, parent_node);
  node_list_.push_back(new_node);
  return new_node;
}

std::vector<Vec2D> PathTree::GetNodeList() {
  std::vector<Vec2D> nodes;
  for (auto itr : node_list_) {
    nodes.push_back(itr->GetData());
  }
  return nodes;
}

std::vector<Vec2D> PathTree::GetPathToRoot(PathNode* node) {
  std::vector<Vec2D> path;
  while (node != NULL) {
    ASSERT((node->GetParent() != NULL) != (node == root_));
    path.push_back(node->GetData());
    node = node->GetParent();
  }
  return path;
}

std::vector<Vec2D> PathTree::GetPathToRoot(Vec2D vec) {
  return GetPathToRoot(FindNode(vec));
}

PathNode* PathTree::FindNode(Vec2D vec) {
  for (auto itr : node_list_) {
    if (itr->GetData() == vec) return itr;
  }
  return NULL;
}

} // namespace core
} // namespace mengde
