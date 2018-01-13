#include "path.h"

//
// Path
//

Path::Path() : Path("") {
}

Path::Path(const char * path) : path_(path) {
}

Path::Path(const string& path) : path_(path) {
}

Path::Path(const boost::filesystem::path& path) : path_(path) {
}

Path Path::operator/(const Path& rhs) const {
  return path_ / rhs.path_;
}

Path Path::operator/(const string& rhs) const {
  return path_ / rhs;
}

Path Path::operator/(const char* rhs) const {
  return path_ / rhs;
}

Path& Path::operator/=(const Path& rhs) {
  path_ /= rhs.path_;
  return *this;
}

Path& Path::operator/=(const string& rhs) {
  path_ /= rhs;
  return *this;
}

Path& Path::operator/=(const char* rhs) {
  path_ /= rhs;
  return *this;
}

