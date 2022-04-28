#pragma once

#include <string>
#include <bitmap.hpp>


namespace jr{
namespace img{


auto from_ppm(std::string const& path) -> Bitmap;


}
}