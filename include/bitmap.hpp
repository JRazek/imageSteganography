#pragma once


#include <cstdint>
#include <cstdio>
#include <algorithm>
#include <memory>
#include "utils.hpp"
#include <vector>

namespace jr{
namespace img{

struct BitmapHeader{
	std::uint32_t file_size;
	std::uint32_t first_data_byte;

	std::uint16_t reserved1;
	std::uint16_t reserved2;

	std::uint32_t image_size;

	BitmapHeader(std::vector<std::uint8_t> const& header){
		std::string format(header.begin(), header.begin()+2);
		if(format!="BM") throw std::invalid_argument("unsupported bmp format");
		
		file_size=bytes_to_little_endianess<std::uint32_t>(header.begin()+0x2, 4);
		first_data_byte=bytes_to_little_endianess<std::uint32_t>(header.begin()+0xA, 4);

		reserved1=bytes_to_little_endianess<std::uint16_t>(header.begin()+0x6, 8);
		reserved2=bytes_to_little_endianess<std::uint16_t>(header.begin()+0x8, 10);

		image_size=bytes_to_little_endianess<std::uint32_t>(header.begin()+0x22, 4);
	}
};



}
}

