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

auto encode_bmp(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& message) -> void{
	std::ifstream input_str(input_path, std::ios::binary);
	std::istreambuf_iterator<char> input_stream_it(input_str);

	std::vector<std::uint8_t> buffer_input(input_stream_it, std::istreambuf_iterator<char>());
	std::vector<std::uint8_t> buffer_output;

	encode<ImageFormat::bmp>(buffer_input, message, std::back_inserter(buffer_output));

	std::ofstream output_str(output_path, std::ios::binary);
	std::ostreambuf_iterator<char> output_stream_it(output_str);

	std::ranges::copy(buffer_output, output_stream_it);
}


auto decode_bmp(std::string const& input_path) -> std::vector<std::uint8_t>{
	std::ifstream input_stream(input_path, std::ios::binary);

	std::istreambuf_iterator<char> input_stream_it(input_stream);
	std::vector<std::uint8_t> buffer_input(input_stream_it, std::istreambuf_iterator<char>());

	std::vector<std::uint8_t> result;
	return decode<ImageFormat::bmp>(buffer_input, std::back_inserter(result));
}


auto encode_ppm(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& message) -> void{
	std::ifstream input_str(input_path, std::ios::binary);
	std::istreambuf_iterator<char> input_stream_it(input_str);

	std::vector<std::uint8_t> buffer_input(input_stream_it, std::istreambuf_iterator<char>());
	std::vector<std::uint8_t> buffer_output;

	encode<ImageFormat::ppm>(buffer_input, message, std::back_inserter(buffer_output));

	std::ofstream output_str(output_path, std::ios::binary);
	std::ostreambuf_iterator<char> output_stream_it(output_str);

	std::ranges::copy(buffer_output, output_stream_it);
}


auto decode_ppm(std::string const& input_path) -> std::vector<std::uint8_t>{
	std::ifstream input_stream(input_path, std::ios::binary);

	std::istreambuf_iterator<char> input_stream_it(input_stream);
	std::vector<std::uint8_t> buffer_input(input_stream_it, std::istreambuf_iterator<char>());

	std::vector<std::uint8_t> result;
	return decode<ImageFormat::ppm>(buffer_input, std::back_inserter(result));
}

}
}

