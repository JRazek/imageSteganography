#include <iostream>
#include <bitmap.hpp>
#include <image_formatters.hpp>
#include <fstream>

auto main()->int{
    using namespace jr::img;
    
    Bitmap img(10, 10, 3);
    auto file=std::fstream("/home/user/CLionProjects/imageSteganography/test_resources/image.ppm");
    read_ppm(std::move(file));
    // auto it=img.begin();

}