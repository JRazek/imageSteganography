#include <iostream>
#include <bitmap.hpp>
#include <image_formatters.hpp>
#include <fstream>
#include <cassert>

auto main()->int{
    using namespace jr::img;
    Bitmap img(10, 10, 3);
    auto file=std::fstream("/home/user/CLionProjects/imageSteganography/test_resources/image.ppm");

    img=read_ppm(std::move(file));

}

