#pragma once


#include <cstdio>
#include <algorithm>
#include <memory>

namespace jr{
namespace img{

enum class ImageFormatting{
    RBG
};

class Bitmap{
    std::size_t _h, _w, _d;

    ImageFormatting _format;

    std::unique_ptr<std::uint8_t[]> _data;
    
public:
 
 
    Bitmap();

    Bitmap(const std::size_t h, const std::size_t w, const std::size_t d);

    Bitmap(const std::size_t h, const std::size_t w, const std::size_t d, std::unique_ptr<std::uint8_t[]> data);

    Bitmap(const std::size_t h, const std::size_t w, const std::size_t d, const ImageFormatting format);

    Bitmap(const std::size_t h, const std::size_t w, const std::size_t d, const ImageFormatting format, std::unique_ptr<std::uint8_t[]> data);
    
    auto set_format(ImageFormatting format) noexcept -> void;
    
    auto set_data(std::unique_ptr<std::uint8_t[]> data) noexcept -> void;

    auto size() const noexcept -> std::size_t;

struct Iterator{
    using iterator_category=std::forward_iterator_tag;
    using differece_type=std::ptrdiff_t;
    using value_type=std::uint8_t;
    using pointer=value_type*;
    using reference=value_type&;

    Iterator(pointer ptr);

    auto operator*() -> reference;

    auto operator->() -> pointer;
    auto operator++() -> Iterator& ;

    friend auto operator==(Iterator const& rhs, Iterator const& lhs);
    friend auto operator!=(Iterator const& rhs, Iterator const& lhs);

private:
    pointer _ptr;
};

    // auto begin() noexcept -> Iterator;

    // auto end() noexcept -> Iterator;
    
};


}
}