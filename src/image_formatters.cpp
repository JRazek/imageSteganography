#include <algorithm>
#include <cstddef>
#include <cstdint>
#include "image_formatters.hpp"
#include "bitmap.hpp"
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

#include "utils.hpp"
#include <istream>
#include <range/v3/view.hpp>


namespace jr{
namespace img{

auto decode_bmp(std::istream&& input) -> std::vector<std::uint8_t>;

auto decode_ppm(std::istream&& input) -> std::vector<std::uint8_t>;


auto encode_bmp(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& message) -> void{
	std::ifstream input_str(input_path, std::ios::binary);
	std::istreambuf_iterator<char> input_stream_it(input_str);

	std::vector<std::uint8_t> buffer_input(input_stream_it, std::istreambuf_iterator<char>());
	std::vector<std::uint8_t> buffer_output;

	encode_bmp(buffer_input, message, std::back_inserter(buffer_output));

	std::ofstream output_str(output_path, std::ios::binary);
	std::ostreambuf_iterator<char> output_stream_it(output_str);

	std::ranges::copy(buffer_output, output_stream_it);
}

auto decode_bmp(std::string const& input_path) -> std::vector<std::uint8_t>{
	std::ifstream input_str(input_path, std::ios::binary);
	return decode_bmp(std::move(input_str));
}


auto decode_bmp(std::istream&& input) -> std::vector<std::uint8_t>{
	std::istreambuf_iterator<char> input_stream_it(input);

	std::vector<std::uint8_t> image_buffered(input_stream_it, std::istreambuf_iterator<char>());

	BMPHeader header{image_buffered};

	std::vector<std::uint8_t> result;

	auto span=std::span{image_buffered.begin()+header.first_data_byte, image_buffered.end()};

	decode_message(span, std::back_inserter(result));

	return result;
}

auto encode_ppm(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& message) -> void{
	std::ifstream input_str(input_path, std::ios::binary);
	std::istreambuf_iterator<char> input_stream_it(input_str);

	std::vector<std::uint8_t> buffer_input(input_stream_it, std::istreambuf_iterator<char>());
	std::vector<std::uint8_t> buffer_output;

	encode_ppm(buffer_input, message, std::back_inserter(buffer_output));

	std::ofstream output_str(output_path, std::ios::binary);
	std::ostreambuf_iterator<char> output_stream_it(output_str);

	std::ranges::copy(buffer_output, output_stream_it);
}

auto decode_ppm(std::string const& input_path) -> std::vector<std::uint8_t>{
	std::ifstream input_str(input_path, std::ios::binary);
	return decode_ppm(std::move(input_str));
}

auto decode_ppm(std::istream&& input) -> std::vector<std::uint8_t>{
	
	std::istreambuf_iterator<char> input_stream_it(input);
	
	std::vector<std::uint8_t> image_buffered;

	std::ranges::copy(input_stream_it, std::istreambuf_iterator<char>(), std::back_inserter(image_buffered));

	PPMHeader header{image_buffered};

	auto it=image_buffered.begin()+header.first_data_byte;

	auto span=std::span{it, image_buffered.end()};

	std::vector<std::uint8_t> result;

	decode_message(span, std::back_inserter(result));

	return result;
}

}
}

