#ifndef UTIL_MISC_HELPERS_H_
#define UTIL_MISC_HELPERS_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

// Miscellaneous Helpers

int GenRandom(int);
int GenRandom(int, int);

vector<string> SplitString(const string&, char);

#endif  // UTIL_MISC_HELPERS_H_
