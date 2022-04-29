#include <bitmap.hpp>


namespace jr{
namespace img{


Bitmap::Bitmap()=default;

Bitmap::Bitmap(const std::size_t h, const std::size_t w, const std::size_t d):
_h(h),
_w(w),
_d(d){}

Bitmap::Bitmap(const std::size_t h, const std::size_t w, const std::size_t d, std::unique_ptr<std::uint8_t[]> data):
Bitmap(h, w, d){
    _data=std::move(data);
}

Bitmap::Bitmap(const std::size_t h, const std::size_t w, const std::size_t d, const ImageFormatting format):
Bitmap(h, w, d){
    _format=format;
}

Bitmap::Bitmap(const std::size_t h, const std::size_t w, const std::size_t d, const ImageFormatting format, std::unique_ptr<std::uint8_t[]> data):
Bitmap(w, h, d, std::move(data)){
    _format=format;
}


// struct Bitmap::Iterator{

//     Iterator(pointer ptr):_ptr(ptr){}

//     auto operator*() -> reference {return *_ptr;};

//     auto operator->() -> pointer {return _ptr;};
//     auto operator++() -> Iterator& { ++_ptr; return *this; };//preincrement ++it

//     friend auto operator==(Iterator const& rhs, Iterator const& lhs){ return rhs._ptr==lhs._ptr; };
//     friend auto operator!=(Iterator const& rhs, Iterator const& lhs){ return rhs._ptr!=lhs._ptr; };

// private:
//     pointer _ptr;
// };



// auto Bitmap::begin() noexcept -> Iterator{
//     return Iterator(_data.get());
// }

// auto Bitmap::end() noexcept -> Iterator{
//     return Iterator(_data.get()+size());
// }

auto Bitmap::set_format(ImageFormatting format) noexcept -> void{
    _format=format;
}

auto Bitmap::set_data(std::unique_ptr<std::uint8_t[]> data) noexcept -> void{
    _data=std::move(data);
}

auto Bitmap::size() const noexcept -> std::size_t{
    return _w * _h * _d;
}

}
}