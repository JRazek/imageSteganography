#include <image_formatters.hpp>
#include <bitmap.hpp>
#include <vector>
#include <fstream>


namespace jr{
namespace img{


auto read_ppm(std::fstream&& fstr) -> Bitmap{

    std::string format; fstr>>format;

    Bitmap bmp;

    if(format=="P1" || format=="P4" ){
        bmp.set_format(ImageFormatting::RBG);
    }
    else if(format=="P2" || format=="P5" ){
        bmp.set_format(ImageFormatting::RBG);
    }
    else if(format=="P3" || format=="P6" ){
        bmp.set_format(ImageFormatting::RBG);
    }
    

    return Bitmap();
}



}
}