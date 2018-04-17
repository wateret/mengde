#ifndef PATH_H_
#define PATH_H_

#include <boost/filesystem.hpp>

#include "common.h"

class Path {
 public:
  Path();
  Path(const char*);
  Path(const string&);
  Path(const boost::filesystem::path&);
  string ToString() const { return path().string(); }

  Path  operator/(const Path& rhs) const;
  Path  operator/(const string& rhs) const;
  Path  operator/(const char* rhs) const;
  Path& operator/=(const Path& rhs);
  Path& operator/=(const string& rhs);
  Path& operator/=(const char* rhs);

  const boost::filesystem::path& path() const { return path_; }

 private:
  boost::filesystem::path path_;
};

#endif  // PATH_H_
