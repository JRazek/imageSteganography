#include <image_formatters.hpp>
#include <bitmap.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <array>
#include <stdexcept>
#include <bitset>

namespace jr{
namespace img{


auto read_ppm(std::fstream&& fstr) -> Bitmap{

    auto string_to_format=[](std::string const& str) -> ImageFormatting{
        if(str=="P1" || str=="P4" ) return ImageFormatting::GRAYSCALE;
        else if(str=="P2" || str=="P5" ) return ImageFormatting::GRAYSCALE;
        return ImageFormatting::RBG;
    };

    std::string line;
    auto i=0u;
    std::size_t bmp_index=0u;
    Bitmap bmp(0,0,0);
    jr::vector3_size size;

    while (std::getline(fstr, line)){
        line=line.substr(0, line.find('#'));
        if(!line.empty()){
            auto stream=std::stringstream(line);
            std::cerr<<line<<'\n';
            if(i==0){
                std::string format_str; stream>>format_str;
                bmp.set_format(string_to_format(format_str));
            }
            else if(i==1){
                std::size_t w, h; stream>>w>>h;
                size.x()=w;
                size.y()=h;
            }
            else if(i==2){
                std::size_t d; stream>>d;
                size.z()=d;
                bmp=Bitmap(size);
            }
            else if(bmp.format()==ImageFormatting::RBG){
                uint8_t r, g, b; stream>>r>>g>>b;
                bmp.get_ref(bmp.getVector(bmp_index))=r;
                bmp.get_ref(bmp.getVector(bmp_index+1))=g;
                bmp.get_ref(bmp.getVector(bmp_index+2))=b;
                bmp_index+=3;
            }
            else if(bmp.format()==ImageFormatting::GRAYSCALE){
                uint8_t b; stream>>b;
                bmp.get_ref(bmp.getVector(bmp_index))=b;
                bmp_index++;
            }
            ++i;
        }
    }

    return bmp;
}

auto read_bmp(std::fstream&& fstr) -> Bitmap{
	
	Bitmap bmp(0,0,0);

	std::array<uint8_t, 14> header;
	
	auto validate_header=[](auto const& header){
	
	};

	std::size_t n=0;
	uint8_t byte;
	
	while(fstr >> byte){
		if(n==14) validate_header(header);

		if(n<14) header[n]=byte;
		else{
		
		}
	}

	return bmp;
}

auto encode_message(Bitmap bitmap, std::vector<std::uint8_t> const& data) -> Bitmap{
	auto information_size=data.size()*8;
	auto shift=bitmap.size().dim_product()/information_size;
	
	if(shift==0) throw std::invalid_argument("cannot encode message larger than image!");
	
	std::bitset<8> data_bitset;

	for(auto i=0u, index=0u; i<information_size; i++, index+=shift){
		if(i%8==0){
			data_bitset=std::bitset<8>(data[i/8]);
		}
		auto& image_byte=bitmap.get_ref(bitmap.getVector(index));

		std::bitset<8> image_bitset(image_byte);
		image_bitset[7]=data_bitset[i%8];

		image_byte=image_bitset.to_ulong();
	}

	return bitmap;
}

auto decode_message(Bitmap const& bitmap) -> std::vector<std::uint8_t>{
	return {}; 
}

}
}

