#include "path.h"

//
// Path
//

Path::Path() : Path(string()) {
}

Path::Path(const string& path) : path_(path) {
  LOG_DEBUG("%s", boost::filesystem::current_path().string().c_str());
}

Path::Path(const boost::filesystem::path& path) : path_(path) {
}

Path Path::operator/(const Path& rhs) const {
  return path_ / rhs.path_;
}

Path Path::operator/(const string& rhs) const {
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
