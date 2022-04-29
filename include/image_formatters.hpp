#pragma once

#include <vector>
#include <bitmap.hpp>
#include <string>


namespace jr{
namespace img{

auto read_ppm(std::string const& path) -> Bitmap;


}
}