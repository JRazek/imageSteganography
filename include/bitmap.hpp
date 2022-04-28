#pragma once


#include <cstdio>
#include <algorithm>
#include <memory>

namespace jr{
namespace img{



class Bitmap{
    std::size_t _h, _w, _d;
    std::unique_ptr<std::uint8_t[]> _data;
    
public:
 
 
    Bitmap();

    Bitmap(std::size_t h, std::size_t w, std::size_t d);

    Bitmap(std::size_t h, std::size_t w, std::size_t d, std::unique_ptr<std::uint8_t[]> data);
    
    struct Iterator;
public:

    // auto begin() noexcept -> Iterator;

    // auto end() noexcept -> Iterator;
    
    auto size() const noexcept -> std::size_t;
};


}
}