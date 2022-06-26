#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

#include "utils.hpp"

namespace jr::img {

enum class ImageFormat {
  bmp,
  ppm,
};

/**
 * @brief class used for generic information fetching from different headers.
 */
struct Header {
  virtual auto get_image_size() -> std::size_t = 0;

  virtual auto get_file_size() -> std::size_t = 0;

  virtual auto get_file_format() -> ImageFormat = 0;

  virtual ~Header() = default;
};

struct BMPHeader : Header {
  std::uint32_t file_size;
  std::uint32_t first_data_byte;

  std::uint16_t reserved1;
  std::uint16_t reserved2;

  std::uint32_t image_size;

  explicit BMPHeader(std::istreambuf_iterator<char> input_stream_it)
      : BMPHeader(std::vector<std::uint8_t>(
            input_stream_it, std::istreambuf_iterator<char>())) {}

  explicit BMPHeader(std::vector<std::uint8_t> const& header) {
    if (header.size() < 0x26)
      throw JRException("invalid header size in bitmap");

    std::string format(header.begin(), header.begin() + 2);
    if (format != "BM") throw JRException("unsupported bmp format");

    file_size =
        bytes_to_little_endianness<std::uint32_t>(header.begin() + 0x2, 4);
    first_data_byte =
        bytes_to_little_endianness<std::uint32_t>(header.begin() + 0xA, 4);

    reserved1 =
        bytes_to_little_endianness<std::uint16_t>(header.begin() + 0x6, 8);
    reserved2 =
        bytes_to_little_endianness<std::uint16_t>(header.begin() + 0x8, 10);

    image_size =
        bytes_to_little_endianness<std::uint32_t>(header.begin() + 0x22, 4);
  }

  auto get_image_size() -> std::size_t override { return image_size; }

  auto get_file_size() -> std::size_t override { return file_size; }

  auto get_file_format() -> ImageFormat override { return ImageFormat::bmp; }
};

struct PPMHeader : Header {
  std::uint32_t file_size;
  std::uint32_t first_data_byte;

  std::uint32_t image_size;

  explicit PPMHeader(std::istreambuf_iterator<char> input_stream_it)
      : PPMHeader(std::vector<std::uint8_t>(
            input_stream_it, std::istreambuf_iterator<char>())) {}

  explicit PPMHeader(std::vector<std::uint8_t> const& header) {
    if (header.size() < 0x2)
      throw JRException("invalid header size in ppm " +
                        std::to_string(header.size()));

    std::string format(header.begin(), header.begin() + 0x2);
    if (format != "P6") throw JRException("invalid format!");

    auto it = header.begin();

    std::size_t line_num = 0;

	std::vector<std::uint8_t> space_separated_nums;

	bool is_comment=false;
    while (it != header.end() && line_num < 3) {

      if (*it == '\n') {
		++it;
        if (it != header.end() && *it != '#'){
          ++line_num;  // this will ignore all lines starting with #
		  is_comment=false;
		}
      }else{
		  if(*it == '#'){ is_comment=true; }
		  else if(!is_comment && line_num>0){
			space_separated_nums.push_back(*it);
		  }
		  ++it;
	  }
	
    }

	image_size=1;

	space_separated_nums.push_back(' ');

	std::uint32_t num=0;

	for(auto ch : space_separated_nums){
		if(ch!=' '){
			num *= 10;
			num += ch-'0';
		}
		else{
			image_size *= num;
			num=0;
		}
	}

    if (line_num != 3) throw JRException("ppm P6 corrupted!");

    file_size = header.size();
    first_data_byte = std::ranges::distance(header.begin(), it);
  }

  auto get_image_size() -> std::size_t override { return image_size; }

  auto get_file_size() -> std::size_t override { return file_size; }

  auto get_file_format() -> ImageFormat override { return ImageFormat::ppm; }
};

/**
 * @brief - detects format and creates polymorphic header type containing
 * required information about file
 * @throw - throws JRException if file is none the supported formats of io error
 * has occurred
 * @param file to detect
 *
 * @return polymorphic header type
 */
auto detectAndCreate(std::string const& file) -> std::unique_ptr<Header>;

}  // namespace jr::img

