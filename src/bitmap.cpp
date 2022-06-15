#include "bitmap.hpp"
#include <algorithm>
#include <iterator>
#include <numeric>
#include <cassert>
#include <iterator>
#include <istream>


namespace jr{
namespace img{


auto detectAndCreate(std::istream&& input_stream) -> std::unique_ptr<Header>{
	std::istreambuf_iterator<char> it(input_stream);
	return detectAndCreate(std::vector<std::uint8_t>(it, std::istreambuf_iterator<char>()));
}

auto detectAndCreate(std::vector<std::uint8_t> const& header_bytes) -> std::unique_ptr<Header>{
	std::unique_ptr<Header> header;
	
	try{
		header=std::make_unique<BMPHeader>(header_bytes);
		return header;
	}catch(...){}

	header=std::make_unique<PPMHeader>(header_bytes); //last without try to throw exception higher

	return header;
}


}
}

