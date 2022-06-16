#pragma once


#include <cstdint>
#include <cstdio>
#include <algorithm>
#include <memory>
#include "utils.hpp"
#include <stdexcept>
#include <string>
#include <vector>

namespace jr{
namespace img{

enum class ImageFormat{
	bmp,
	ppm,
};


/**
 * @brief class used for generic information fetching from different headers.
 */
struct Header{
	virtual auto get_image_size() -> std::size_t = 0;

	virtual auto get_file_size() -> std::size_t = 0;

	virtual auto get_file_format() -> ImageFormat =0;

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
		if(header.size()<0x26) throw JRException("invalid header size in bitmap");

		std::string format(header.begin(), header.begin()+2);
		if(format!="BM") throw JRException("unsupported bmp format");
		
		file_size=bytes_to_little_endianess<std::uint32_t>(header.begin()+0x2, 4);
		first_data_byte=bytes_to_little_endianess<std::uint32_t>(header.begin()+0xA, 4);

		reserved1=bytes_to_little_endianess<std::uint16_t>(header.begin()+0x6, 8);
		reserved2=bytes_to_little_endianess<std::uint16_t>(header.begin()+0x8, 10);

		image_size=bytes_to_little_endianess<std::uint32_t>(header.begin()+0x22, 4);
	}
	
	auto get_image_size() -> std::size_t override { return image_size; }

	auto get_file_size() -> std::size_t override { return file_size; }

	auto get_file_format() -> ImageFormat override { return ImageFormat::bmp; }
};

struct PPMHeader : Header{
	std::uint32_t file_size;
	std::uint32_t first_data_byte;

	std::uint32_t image_size;

	PPMHeader(std::istreambuf_iterator<char> input_stream_it):
	PPMHeader(std::vector<std::uint8_t>(input_stream_it, std::istreambuf_iterator<char>()))
	{}

	PPMHeader(std::vector<std::uint8_t> const& header){
		if(header.size()<0x2) throw JRException("invalid header size in ppm " + std::to_string(header.size()));

		std::string format(header.begin(), header.begin()+0x2);
		if(format!="P6") throw JRException("invalid format!");

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
		
		if(line_num!=3) throw JRException("ppm P6 corrupted!");

		file_size=header.size();
		first_data_byte=std::ranges::distance(header.begin(), it);
	}

	auto get_image_size() -> std::size_t override { return image_size; }

	auto get_file_size() -> std::size_t override { return file_size; }

	auto get_file_format() -> ImageFormat override { return ImageFormat::ppm; }
};


/**
* @brief - detects format and creates polymorphic header type containing required information about file
* @throw - throws JRException if file is none the supported formats of io error has occured
* @param file to detect
*
* @return polymorphic header type
*/
auto detectAndCreate(std::string const file) -> std::unique_ptr<Header>;


}
}

