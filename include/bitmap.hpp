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
public:

    using data_type=uint8_t;

    using Container=std::vector<data_type>;
 
    Bitmap();

    Bitmap(jr::vector3_size const size);

    Bitmap(std::size_t const h, std::size_t const w, std::size_t const d);

    auto set_format(ImageFormatting const format) noexcept -> void;

    auto format() const noexcept -> ImageFormatting;

    auto set_size(jr::vector3_size const size) noexcept -> void;
    
    auto set_data(Container const& data) noexcept -> void;

    auto set_data(Container&& data) noexcept -> void;

    auto size() const noexcept -> std::size_t;

    auto get(jr::vector3_size const vec) noexcept -> data_type&;

    auto get(jr::vector3_size const vec) const noexcept -> data_type const&;

    auto begin() noexcept -> Container::iterator;

    auto end() noexcept -> Container::iterator;
    
    inline auto getIndex(jr::vector3_size const vec) const noexcept -> std::size_t;

private:

    jr::vector3_size _size;

    ImageFormatting _format;

    Container _data;
    
};


}
}