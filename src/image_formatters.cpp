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

auto check_encodable_bmp(std::string const& input_path, std::string const& output_path, std::vector<std::uint8_t> const& message) -> bool{
	std::ifstream input_str(input_path, std::ios::binary);
	std::istreambuf_iterator<char> input_stream_it(input_str);

	std::vector<std::uint8_t> buffer_input(input_stream_it, std::istreambuf_iterator<char>());

	try{
		SteganographyImageMetadata::create_metadata_for_encrypting(buffer_input, message);
	}catch(...){ return false; }

	return true;
}

}
}

