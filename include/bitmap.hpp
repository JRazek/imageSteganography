#pragma once


#include <cstdint>
#include <cstdio>
#include <algorithm>
#include <memory>
#include "utils.hpp"
#include <stdexcept>
#include <vector>

namespace jr{
namespace img{

struct Header{
	virtual auto get_image_size() -> std::size_t = 0;

	virtual auto get_file_size() -> std::size_t = 0;

	virtual ~Header() = default;

};

struct BMPHeader : Header{
	std::uint32_t file_size;
	std::uint32_t first_data_byte;

	std::uint16_t reserved1;
	std::uint16_t reserved2;

	std::uint32_t image_size;

	BMPHeader(std::istreambuf_iterator<char> input_stream_it):
	BMPHeader(std::vector<std::uint8_t>(input_stream_it, std::istreambuf_iterator<char>()))
	{}

	BMPHeader(std::vector<std::uint8_t> const& header){
		if(header.size()<0x26) throw std::invalid_argument("invalid header size in bitmap");

		std::string format(header.begin(), header.begin()+2);
		if(format!="BM") throw std::invalid_argument("unsupported bmp format");
		
		file_size=bytes_to_little_endianess<std::uint32_t>(header.begin()+0x2, 4);
		first_data_byte=bytes_to_little_endianess<std::uint32_t>(header.begin()+0xA, 4);

		reserved1=bytes_to_little_endianess<std::uint16_t>(header.begin()+0x6, 8);
		reserved2=bytes_to_little_endianess<std::uint16_t>(header.begin()+0x8, 10);

		image_size=bytes_to_little_endianess<std::uint32_t>(header.begin()+0x22, 4);
	}
	
	auto get_image_size() -> std::size_t override { return image_size; }

	auto get_file_size() -> std::size_t override { return file_size; }
};

struct PPMHeader : Header{
	std::uint32_t file_size;
	std::uint32_t first_data_byte;

	std::uint32_t image_size;

	PPMHeader(std::istreambuf_iterator<char> input_stream_it):
	PPMHeader(std::vector<std::uint8_t>(input_stream_it, std::istreambuf_iterator<char>()))
	{}

	PPMHeader(std::vector<std::uint8_t> const& header){
		if(header.size()<0x2) throw std::invalid_argument("invalid header size in bitmap");

		std::string format(header.begin(), header.begin()+0x2);
		if(format!="P6") throw std::invalid_argument("invalid format!");

		auto it=header.begin();

		std::size_t line_num=0; 

		auto test=0;
		
		while(it!=header.end() && line_num<3){
			if(*it=='\n'){
				if(it!=header.end() && *it!='#') ++line_num;//this will ignore all lines starting with #
			}
			++it;
			test++;
		}
		
		if(line_num!=3) throw std::invalid_argument("ppm P6 corrupted!");

		file_size=header.size();
		first_data_byte=std::ranges::distance(header.begin(), it);
	}

	auto get_image_size() -> std::size_t override { return image_size; }

	auto get_file_size() -> std::size_t override { return file_size; }
};

auto detectAndCreate(std::istream&& input_stream) -> std::unique_ptr<Header>;
auto detectAndCreate(std::vector<std::uint8_t> const& header_bytes) -> std::unique_ptr<Header>;


}
}

