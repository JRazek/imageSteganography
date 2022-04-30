#include <bitmap.hpp>
#include <algorithm>
#include <numeric>


namespace jr{
namespace img{


Bitmap::Bitmap()=default;

Bitmap::Bitmap(const jr::vector3_size size): _size(size){}

Bitmap::Bitmap(const std::size_t h, const std::size_t w, const std::size_t d){}

// Bitmap::Bitmap(const std::size_t h, const std::size_t w, const std::size_t d, std::unique_ptr<std::uint8_t[]> data):
// Bitmap(h, w, d){
//     _data=std::move(data);
// }

// Bitmap::Bitmap(const std::size_t h, const std::size_t w, const std::size_t d, const ImageFormatting format):
// Bitmap(h, w, d){
//     _format=format;
// }

// Bitmap::Bitmap(const std::size_t h, const std::size_t w, const std::size_t d, const ImageFormatting format, std::unique_ptr<std::uint8_t[]> data):
// Bitmap(w, h, d, std::move(data)){
//     _format=format;
// }


auto Bitmap::set_format(const ImageFormatting format) noexcept -> void{
    _format=format;
}

auto Bitmap::set_size(const jr::vector3_size size) noexcept -> void{
    _size=size;
}

auto Bitmap::set_data(std::unique_ptr<std::uint8_t[]> data) noexcept -> void{
    _data=std::move(data);
}

auto Bitmap::size() const noexcept -> std::size_t{
    return _size.dim_product();
}

auto Bitmap::operator[](std::size_t y) noexcept -> Iterator{
    return begin() + y*_size.y();
}

// auto Bitmap::operator[](std::size_t y) noexcept -> std::uint8_t&{

// }


auto Bitmap::begin() noexcept -> Iterator{
    return Iterator(_data.get());
}

auto Bitmap::end() noexcept -> Iterator{
    return Iterator(_data.get()+size());
}

}
}