#include "multibase.h"
#include <multibase/multibase.h>

namespace multibase {

std::string encode(const cstring_span& input, encoding base,
                   bool include_encoding) {
  return codec(base).encode(input, include_encoding);
}

std::size_t encode(const cstring_span& input, string_span& output,
                   encoding base, bool include_encoding) {
  return codec(base).encode(input, output, include_encoding);
}

std::size_t encoded_size(const cstring_span& input, encoding base,
                         bool include_encoding) {
  return codec(base).encoded_size(input, include_encoding);
}

std::string decode(const cstring_span& input, encoding base) {
  if (input.empty()) return std::string();
  auto mb = multibase(input, base);
  auto decoder = codec(mb.base());
  auto view = cstring_span(&input[0] + 1, input.size() - 1);
  return decoder.decode(view);
}

std::size_t decode(const cstring_span& input, string_span& output,
                   encoding base) {
  auto mb = multibase(input, base);
  return codec(mb.base()).decode(mb.data(), output);
}

std::size_t decoded_size(const cstring_span& input, encoding base) {
  auto mb = multibase(input, base);
  return codec(mb.base()).decoded_size(input);
}

bool is_valid(const cstring_span& input, encoding base) {
  auto result = true;
  try {
    auto mb = multibase(input, base);
    result = is_valid(mb);
  } catch (std::range_error& e) {
    result = false;
  }
  return result;
}

bool is_valid(const multibase& value) {
  return codec(value.base()).is_valid(value.data(), false);
}

multibase::multibase(cstring_span data, encoding base)
    : data_(data), base_(base) {
  if (base_ == encoding::base_unknown) {
    if (data_.length() < 2) {
      throw std::range_error("multibase must be at least 2 characters");
    }
    base_ = static_cast<encoding>(data_[0]);
    data_ = cstring_span(&data_[1], data_.size() - 1);
  }
}

encoding multibase::base() const { return base_; }

cstring_span multibase::data() const { return data_; }

bool multibase::operator==(const multibase& rhs) const {
  return data_ == rhs.data_;
}

bool multibase::operator!=(const multibase& rhs) const {
  return !(*this == rhs);
}

bool multibase::operator<(const multibase& rhs) const {
  return data_ < rhs.data_;
}

bool multibase::operator>(const multibase& rhs) const {
  return data_ > rhs.data_;
}
}  // namespace multibase
