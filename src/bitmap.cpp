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

auto Bitmap::format() const noexcept -> ImageFormatting{
    return _format;
}

auto Bitmap::set_size(jr::vector3_size const size) noexcept -> void{
    _size=size;
}

auto Bitmap::set_data(Container const& data) noexcept -> void{
    assert(data.size()==_size.dim_product());
    _data=data;
}

auto Bitmap::set_data(Container&& data) noexcept -> void{
    assert(data.size()==_size.dim_product());
    _data=std::move(data);
}

auto Bitmap::size() const noexcept -> std::size_t{
    return _size.dim_product();
}

auto Bitmap::get(jr::vector3_size const vec) noexcept -> data_type&{
    return _data[getIndex(vec)];
}

auto Bitmap::get(jr::vector3_size const vec) const noexcept -> data_type const&{
    assert(vec.x()>0 && vec.x()<_size.x() && vec.y()>0 && vec.y()<_size.y() && vec.z()>0 && vec.z()<_size.z());
    return _data[getIndex(vec)];
}


auto Bitmap::begin() noexcept -> Container::iterator{
    return _data.begin();
}

auto Bitmap::end() noexcept -> Container::iterator{
    return _data.end();
}

inline auto Bitmap::getIndex(jr::vector3_size const vec) const noexcept -> std::size_t{
    assert(vec.x()>0 && vec.x()<_size.x() && vec.y()>0 && vec.y()<_size.y() && vec.z()>0 && vec.z()<_size.z());
    return vec.y()*_size.z()*_size.x() + vec.x()*_size.z() + vec.z();
}

}
}