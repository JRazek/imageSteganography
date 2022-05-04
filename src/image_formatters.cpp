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
    Bitmap bmp;
    jr::vector3_size size;

    while (std::getline(fstr, line)){
        line=line.substr(0, line.find('#'));
        if(!line.empty()){
            auto stream=std::stringstream(line);
            std::cout<<line<<'\n';
            if(i==0){
                std::string format_str; stream>>format_str;
                bmp.set_format(string_to_format(format_str));
            }else if(i==1){
                std::size_t w, h; stream>>w>>h;
                size.x()=w;
                size.y()=h;
            }else if(i==2){
                std::size_t d; stream>>d;
                size.z()=d;
                bmp.set_size(size);
            }else{
                
            }
            ++i;
        }
    }
    
    
    

    return bmp;
}



}
}