#pragma once


#include <cstdio>
#include <algorithm>
#include <memory>
#include <utils.h>

namespace jr{
namespace img{

enum class ImageFormatting{
    GRAYSCALE,
    RBG,
};

class Bitmap{

    jr::vector3_size _size;

    ImageFormatting _format;

    std::unique_ptr<std::uint8_t[]> _data;
    
public:
 
 
    Bitmap();

    Bitmap(const jr::vector3_size size);

    Bitmap(const std::size_t h, const std::size_t w, const std::size_t d);

    auto set_format(const ImageFormatting format) noexcept -> void;

    auto set_size(const jr::vector3_size size) noexcept -> void;
    
    auto set_data(std::unique_ptr<std::uint8_t[]> data) noexcept -> void;

    auto size() const noexcept -> std::size_t;


struct Iterator{
    using iterator_category=std::forward_iterator_tag;
    using differece_type=std::ptrdiff_t;
    using value_type=std::uint8_t;
    using pointer=value_type*;
    using reference=value_type&;

    Iterator(pointer ptr):_ptr(ptr){}

    auto operator*() -> reference {return *_ptr;};

    auto operator->() -> pointer {return _ptr;};
    auto operator++() -> Iterator& { ++_ptr; return *this; };//preincrement ++it

    friend auto operator+(Iterator rhs, std::size_t shift){ 
        Iterator it=rhs; 
        it._ptr+=shift;
        return it; 
    };
    
    friend auto operator==(Iterator const& rhs, Iterator const& lhs){ return rhs._ptr==lhs._ptr; };
    friend auto operator!=(Iterator const& rhs, Iterator const& lhs){ return rhs._ptr!=lhs._ptr; };

private:
    pointer _ptr;
};

    auto operator[](std::size_t y) noexcept -> Iterator;

    auto begin() noexcept -> Iterator;

    auto end() noexcept -> Iterator;
    
};


}
}