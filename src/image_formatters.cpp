#include <image_formatters.hpp>
#include <bitmap.hpp>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

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
                bmp.get(bmp.getVector(bmp_index))=r;
                bmp.get(bmp.getVector(bmp_index+1))=g;
                bmp.get(bmp.getVector(bmp_index+2))=b;
                bmp_index+=3;
            }
            else if(bmp.format()==ImageFormatting::GRAYSCALE){
                uint8_t b; stream>>b;
                bmp.get(bmp.getVector(bmp_index))=b;
                bmp_index++;
            }
            ++i;
        }
    }

    return bmp;
}



}
}