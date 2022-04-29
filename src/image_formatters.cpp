#include <image_formatters.hpp>
#include <bitmap.hpp>
#include <vector>
#include <fstream>


namespace jr{
namespace img{


auto read_ppm(std::string const& path) -> Bitmap{
    
    std::fstream fstr(path, std::ios::in);

    std::string format; fstr>>format;


    Bitmap bmp;
    if(format=="P3"){
        bmp.set_format(ImageFormatting::RBG);
    }
    

    return Bitmap();
}



}
}