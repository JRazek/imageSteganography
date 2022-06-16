#include "header_utils.hpp"

#include <algorithm>
#include <cassert>
#include <exception>
#include <fstream>
#include <istream>
#include <iterator>
#include <numeric>
#include <string>

#include "utils.hpp"

namespace jr {
namespace img {

/**
 * @brief - detects format and creates polymorphic header type containing
 * required information about file
 * @throw - throws JRException if file is none the supported formats of io error
 * has occured
 * @param - buffered header bytes
 *
 * @return polymorphic header type
 */
auto detectAndCreate(std::vector<std::uint8_t> const& header_bytes)
    -> std::unique_ptr<Header> {
  std::unique_ptr<Header> header;

  try {
    header = std::make_unique<BMPHeader>(header_bytes);
    return header;
  } catch (...) {
  }

  header = std::make_unique<PPMHeader>(
      header_bytes);  // last without try to throw exception higher

  return header;
}

auto detectAndCreate(std::string const file) -> std::unique_ptr<Header> {
  std::ifstream input_stream(file, std::ios::binary);
  if (input_stream.fail())
    throw JRException("io exception while reading file!");

  std::istreambuf_iterator<char> it(input_stream);
  std::vector<std::uint8_t> input_buffer(it, std::istreambuf_iterator<char>());
  return detectAndCreate(input_buffer);
}

}  // namespace img
}  // namespace jr

