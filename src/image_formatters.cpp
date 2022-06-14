#include <algorithm>
#include <cstdint>
#include <image_formatters.hpp>
#include <bitmap.hpp>
#include <istream>
#include <iterator>
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
#include <range/v3/view.hpp>


namespace jr{
namespace img{

auto encode_bmp(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& data) -> void{
	std::ifstream input_str(input_path, std::ios::binary);
	std::ofstream output_str(output_path, std::ios::binary | std::ios::trunc);

	encode_bmp(std::move(input_str), std::move(output_str), data);
}

auto encode_bmp(std::istream&& input, std::ostream&& output, std::vector<std::uint8_t> const& message) -> void{
	constexpr auto header_size=54;

	std::vector<std::uint8_t> header_bytes(header_size);

	input.read(reinterpret_cast<char*>(header_bytes.data()), header_size);

	BitmapHeader header{header_bytes};

	std::vector<std::uint8_t> metadata(header.first_data_byte-header_size);

	input.read(reinterpret_cast<char*>(metadata.data()), metadata.size());

	std::istreambuf_iterator<char> input_stream_it(input); 

	std::vector<std::uint8_t> image_buffered(input_stream_it, std::istreambuf_iterator<char>());

	encode_message(image_buffered, message, image_buffered.begin());

	std::ostreambuf_iterator<char> output_stream_it(output); 
	
	for(auto b : ranges::views::concat(header_bytes, metadata, image_buffered)) 
		*output_stream_it=b;
}

auto decode_bmp(std::string const& input_path) -> std::vector<std::uint8_t>{
	std::ifstream input_str(input_path, std::ios::binary);
	return decode_bmp(std::move(input_str));
}


auto decode_bmp(std::istream&& input) -> std::vector<std::uint8_t>{
	constexpr auto header_size=54;

	std::vector<std::uint8_t> header_bytes(header_size);

	input.read(reinterpret_cast<char*>(header_bytes.data()), header_size);

	BitmapHeader header{header_bytes};

	std::vector<std::uint8_t> metadata(header.first_data_byte-header_size);

	input.read(reinterpret_cast<char*>(metadata.data()), metadata.size());

	std::istreambuf_iterator<char> input_stream_it(input); 

	std::vector<std::uint8_t> image_buffered(input_stream_it, std::istreambuf_iterator<char>());

	std::vector<std::uint8_t> result;

	decode_message(image_buffered, std::back_inserter(result));

	return result;
}



}
}

