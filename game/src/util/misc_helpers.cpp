#include "misc_helpers.h"
#include <sstream>
#include "stdlib.h"

int GenRandom(int v) { return rand() % v; }

// return an integer between [begin, end)
int GenRandom(int begin, int end) { return begin + GenRandom(end - begin); }

vector<string> SplitString(const string& str, char sep) {
  std::stringstream ss(str);
  string            elem;
  vector<string>    ret;
  while (std::getline(ss, elem, sep)) {
    ret.push_back(std::move(elem));
  }
  return ret;
}
