#pragma once

#include <vector>
#include <bitmap.hpp>
#include <string>


namespace jr{
namespace img{

auto read_ppm(std::istream&& str) -> PPM;

auto read_ppm(std::string const& file) -> PPM;

auto write_ppm(PPM const& ppm, std::ostream&& str) -> void;

auto read_bmp(std::istream&& str) -> Bitmap;

auto encode_message(Bitmap bitmap, std::vector<std::uint8_t> const& data) -> Bitmap;

template<typename Image>
auto encode_message(Bitmap bitmap, std::vector<std::uint8_t> const& data) -> Image{
	return Image(encode_message(bitmap, data));
}

/*
 * Message must be previously encoded with encode_message function!
 * In order to keep the original message untouched, bitmap state from any of read_* functions,
 * has to stay unchanged.
 */
auto decode_message(Bitmap const& bitmap) -> std::vector<std::uint8_t>;


auto encode_bmp(std::string const& path) -> void;

}

}

