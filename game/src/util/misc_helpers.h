#ifndef MISC_HELPERS_H_
#define MISC_HELPERS_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

// Miscellaneous Helpers

int GenRandom(int);
int GenRandom(int, int);

vector<string> SplitString(const string&, char);

#endif  // MISC_HELPERS_H_
