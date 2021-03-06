#pragma once

#include <cstddef>
#include <cstring>
#include <string>

namespace pidan {

// 借鉴了LevelDB的Slice
class Slice {
 public:
  Slice() : data_(""), size_(0) {}

  Slice(const char *d, size_t n) : data_(d), size_(n) {}

  Slice(const std::string &s) : data_(s.data()), size_(s.size()) {}

  Slice(const char *s) : data_(s), size_(strlen(s)) {}

  Slice(const Slice &) = default;

  Slice &operator=(const Slice &) = default;

  const char *data() const { return data_; }

  size_t size() const { return size_; }

  bool empty() const { return size_ == 0; }

  void clear() {
    data_ = "";
    size_ = 0;
  }

  std::string ToString() const { return std::string(data_, size_); }

  int compare(const Slice& b) const;
  
 private:
  const char *data_;
  size_t size_;
};

inline int Slice::compare(const Slice& b) const {
  const size_t min_len = (size_ < b.size_) ? size_ : b.size_;
  int r = memcmp(data_, b.data_, min_len);
  if (r == 0) {
    if (size_ < b.size_)
      r = -1;
    else if (size_ > b.size_)
      r = +1;
  }
  return r;
}

inline bool operator<(const Slice &x, const Slice &y) {
  const size_t min_len = (x.size() < y.size()) ? x.size() : y.size();
  int r = std::memcmp(x.data(), y.data(), min_len);
  if (r != 0) {
    return r < 0;
  }
  return x.size() < y.size();
}

inline bool operator>(const Slice &x, const Slice &y) { return y < x; }

inline bool operator>=(const Slice &x, const Slice &y) { return !(x < y); }

inline bool operator<=(const Slice &x, const Slice &y) { return !(x > y); }

inline bool operator==(const Slice &x, const Slice &y) {
  return ((x.size() == y.size()) && (std::memcmp(x.data(), y.data(), x.size()) == 0));
}

inline bool operator!=(const Slice &x, const Slice &y) { return !(x == y); }

}  // namespace pidan