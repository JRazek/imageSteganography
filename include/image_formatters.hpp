#pragma once

#include <vector>
#include <bitmap.hpp>
#include <string>


namespace jr{
namespace img{

auto read_ppm(std::fstream&& fstr) -> Bitmap;

auto read_bmp(std::fstream&& fstr) -> Bitmap;

auto encode_message(Bitmap bitmap, std::vector<std::uint8_t> const& data) -> Bitmap;

/*
 * Message must be previously encoded with encode_message function!
 * In order to keep the original message untouched, bitmap state from any of read_* functions,
 * has to stay unchanged.
 */
auto decode_message(Bitmap const& bitmap) -> std::vector<std::uint8_t>;

}

}

