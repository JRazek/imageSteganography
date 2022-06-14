#include <cstdint>
#include <image_formatters.hpp>
#include <bitmap.hpp>
#include <istream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <array>
#include <stdexcept>
#include <bitset>
#include <cassert>
#include <optional>

#include <utils.hpp>
#include <istream>


namespace jr{
namespace img{

auto decode_bmp(std::istream&& str) -> std::vector<std::uint8_t>{

	constexpr auto header_size=54;

	std::vector<std::uint8_t> header_bytes(header_size);

	str.read(reinterpret_cast<char*>(header_bytes.data()), header_size);

	BitmapHeader header{header_bytes};

	std::istream_iterator<std::uint8_t> low(str); std::ranges::advance(low, header.first_data_byte);
	auto high=low; std::ranges::advance(high, header.image_size);

	std::vector<std::uint8_t> image_raw(low, high);
	std::vector<std::uint8_t> result;

	decode_message(image_raw, std::back_inserter(result));

	return result;
}

auto encode_bmp(std::string const& path, std::vector<std::uint8_t> const& data) -> void{
	encode_bmp(std::fstream(path, std::ios::in | std::ios::out | std::ios::binary), data);
}

auto encode_bmp(std::iostream&& str, std::vector<std::uint8_t> const& data) -> void{
	constexpr auto header_size=54;

	std::vector<std::uint8_t> header_bytes(header_size);

	str.read(reinterpret_cast<char*>(header_bytes.data()), header_size);

	BitmapHeader header{header_bytes};

	std::istream_iterator<std::uint8_t> in(str); std::advance(in, header.first_data_byte);
	std::ostream_iterator<std::uint8_t> out(str); for(auto i=0u;i<header.first_data_byte;i++) ++out;

//	encode_message(in, header.image_size, out, data);
}



}
}

