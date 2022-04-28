#include <bitmap.hpp>


namespace jr{
namespace img{


Bitmap::Bitmap()=default;

Bitmap::Bitmap(std::size_t h, std::size_t w, std::size_t d):_h(h), _w(w), _d(d){}

Bitmap::Bitmap(std::size_t h, std::size_t w, std::size_t d, std::unique_ptr<std::uint8_t[]> data):
Bitmap(h, w, d){
    _data=std::move(data);
}

struct Bitmap::Iterator{
    using iterator_category=std::forward_iterator_tag;
    using differece_type=std::ptrdiff_t;
    using value_type=std::uint8_t;
    using pointer=value_type*;
    using reference=value_type&;

    Iterator(pointer ptr):_ptr(ptr){}

    auto operator*() -> reference {return *_ptr;};

    auto operator->() -> pointer {return _ptr;};
    auto operator++() -> Iterator& { ++_ptr; return *this; };//preincrement ++it

    friend auto operator==(Iterator const& rhs, Iterator const& lhs){ return rhs._ptr==lhs._ptr; };
    friend auto operator!=(Iterator const& rhs, Iterator const& lhs){ return rhs._ptr!=lhs._ptr; };

private:
    pointer _ptr;
};



// auto Bitmap::begin() noexcept -> Iterator{
//     return Iterator(_data.get());
// }

// auto Bitmap::end() noexcept -> Iterator{
//     return Iterator(_data.get()+size());
// }

auto Bitmap::size() const noexcept -> std::size_t{
    return _w * _h * _d;
}

}
}