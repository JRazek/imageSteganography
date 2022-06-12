#include "utils.hpp"
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


namespace jr{
namespace img{

//[TODO fix - format ignores whitespaces :)]

auto read_ppm(std::istream&& str) -> PPM{
    std::string line;
    std::size_t bmp_index=0u;
    PPM bmp(0,0,0);
    jr::vector3_size size;

	using Format=PPM::PPM_Format;

    auto string_to_format=[](std::string const& str) -> Format{
		//unsupported if(str=="P1") return Format::P1;
		if(str=="P2") return Format::P2;
		if(str=="P3") return Format::P3;
		//unsupported if(str=="P4") return Format::P4;
		//unsupported if(str=="P5") return Format::P5;
		//unsupported if(str=="P6") return Format::P6;

		throw std::invalid_argument("unsupported "+str+" format");
    };

	std::string format_string; str>>format_string;
	Format format=string_to_format(format_string);

	while(!str.eof()){
		
	}

    return bmp;
}

auto read_ppm(std::string const& file) -> PPM{
	std::fstream stream(file, std::ios::in);
	return read_ppm(std::move(stream));
}

auto write_ppm(PPM const& ppm, std::ostream&& fstr) -> void{
	using Format=PPM::PPM_Format;

    auto format_to_string=[](Format const format) -> std::string{
		//unsupported if(format==Format::P1) return "P1";
		if(format==Format::P2) return "P2";
		if(format==Format::P3) return "P3";
		//unsupported if(format==Format::P4) return "P4";
		//unsupported if(format==Format::P5) return "P5";
		//unsupported if(format==Format::P6) return "P6";

		throw std::invalid_argument("unsupported "+std::to_string(static_cast<int>(format))+" format");
    };

	fstr<<format_to_string(ppm.format())<<'\n';

	fstr<<ppm.size().x()<<' '<<ppm.size().y()<<'\n';

	fstr<<ppm.size().z()<<'\n';

	for(auto row=0u; row<ppm.size().y(); row++){
		for(auto col=0u; col<ppm.size().x(); col++){
			for(auto canal=0u; canal<ppm.size().z(); canal++){
				auto val=ppm.get({col, row, canal});
				fstr<<std::to_string(val)<<' ';
			}
		}
		fstr<<'\n';
	}
}

auto read_bmp(std::istream&& str) -> Bitmap{

	std::array<std::uint8_t, 14> header;

	str.read(reinterpret_cast<char*>(header.data()), 14);

	struct BmpHeader{
		std::uint32_t size;
		std::uint32_t first_data_byte;

		std::uint16_t reserved1;
		std::uint16_t reserved2;
		BmpHeader(std::array<std::uint8_t, 14> const& header){
			std::string format(header.begin(), header.begin()+2);
			if(format!="BM") throw std::invalid_argument("invalid bmp format");
			
			size=bytes_to_little_endianess<std::uint32_t>(header.begin()+2, header.begin()+4);
			first_data_byte=bytes_to_little_endianess<std::uint32_t>(header.begin()+10, header.begin()+14);

			reserved1=bytes_to_little_endianess<std::uint16_t>(header.begin()+6, header.begin()+8);
			reserved2=bytes_to_little_endianess<std::uint16_t>(header.begin()+8, header.begin()+10);
		}
	};

	
	Bitmap bmp(0,0,0);



	return bmp;
}

auto encode_message(Bitmap bitmap, std::vector<std::uint8_t> const& data) -> Bitmap{
	auto header_bytes=to_little_endianness_bytes<8>(data.size());

	auto shift=(bitmap.size().dim_product()-8)/(data.size()*8);
	if(!shift) throw std::invalid_argument("data is larger than image!");

	auto it=bitmap.begin();
	for(auto i=0;i<8;i++, ++it) *it=header_bytes[i];

	for(auto b : data){
		std::bitset<8> byte(b);

		for(auto i=0u; i<8;i++, it+=shift){
			auto bitmap_byte=std::bitset<8>(*it);
			bitmap_byte[7]=byte[i];
			
			*it=bitmap_byte.to_ulong();
		}
	}

	return bitmap;
}

auto decode_message(Bitmap const& bitmap) -> std::vector<std::uint8_t>{
	auto it=bitmap.begin();
	
	auto data_size=bytes_to_little_endianess<std::size_t>(it, it+8); it+=8;
	
	auto shift=(bitmap.size().dim_product()-8)/(data_size*8);

	if(!shift) throw std::invalid_argument("encoded image is corrupted!");

	std::vector<std::uint8_t> data(data_size);

	for(auto& data_byte_raw : data){

		std::bitset<8> data_byte(data_byte_raw);

		for(auto i=0u;i<8;i++,it+=shift){
			auto bitmap_byte=std::bitset<8>(*it);
			data_byte[i]=bitmap_byte[7];
		}

		data_byte_raw=data_byte.to_ulong();
	}

	return data;
}




}
}

