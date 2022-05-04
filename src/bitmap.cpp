#include <bitmap.hpp>
#include <algorithm>
#include <numeric>
#include <cassert>


namespace jr{
namespace img{


Bitmap::Bitmap()=default;

Bitmap::Bitmap(jr::vector3_size const size): _size(size){}

Bitmap::Bitmap(std::size_t const h, std::size_t const w, std::size_t const d){}

auto Bitmap::set_format(ImageFormatting const format) noexcept -> void{
    _format=format;
}

auto Bitmap::set_size(jr::vector3_size const size) noexcept -> void{
    _size=size;
}

auto Bitmap::set_data(std::unique_ptr<std::uint8_t[]> data) noexcept -> void{
    _data=std::move(data);
}

auto Bitmap::size() const noexcept -> std::size_t{
    return _size.dim_product();
}


auto Bitmap::get(jr::vector3_size const vec) noexcept -> uint8_t&{
    assert(vec.x()>0 && vec.x()<_size.x() && vec.y()>0 && vec.y()<_size.y() && vec.z()>0 && vec.z()<_size.z());

    // return
}

auto Bitmap::get(jr::vector3_size const vec) const noexcept -> uint8_t const&{
    assert(vec.x()>0 && vec.x()<_size.x() && vec.y()>0 && vec.y()<_size.y() && vec.z()>0 && vec.z()<_size.z());

    //return 
}






auto Bitmap::operator[](std::size_t const y) noexcept -> Iterator{
    return begin() + y*_size.y();
}

auto Bitmap::begin() noexcept -> Iterator{
    return Iterator(_data.get());
}

auto Bitmap::end() noexcept -> Iterator{
    return Iterator(_data.get()+size());
}

}
}