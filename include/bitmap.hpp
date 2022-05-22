#pragma once


#include <cstdio>
#include <algorithm>
#include <memory>
#include <utils.hpp>

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
 

    Bitmap(Bitmap const& bmp) noexcept;

    Bitmap(Bitmap&& bmp) noexcept;

    Bitmap() noexcept;

    Bitmap(jr::vector3_size const size) noexcept;

    Bitmap(std::size_t const h, std::size_t const w, std::size_t const d) noexcept;

    auto set_format(ImageFormatting const format) noexcept -> void;

    auto format() const noexcept -> ImageFormatting;
    
    auto set_data(Container const& data) noexcept -> void;

    auto set_data(Container&& data) noexcept -> void;

    auto size() const noexcept -> jr::vector3_size;

    auto get(jr::vector3_size const vec) noexcept -> data_type;

    auto get_ref(jr::vector3_size const vec) noexcept -> data_type&;

    auto get_ref(jr::vector3_size const vec) const noexcept -> data_type const&;

    auto begin() noexcept -> Container::iterator;

    auto end() noexcept -> Container::iterator;
    
    auto get_index(jr::vector3_size const vec) const noexcept -> std::size_t;

    auto getVector(std::size_t const index) const noexcept -> jr::vector3_size;

    auto operator=(Bitmap const& bmp) noexcept -> Bitmap&;

    auto operator=(Bitmap&& bmp) noexcept -> Bitmap&;

private:

    jr::vector3_size _size;//possible to set only with constructor or move/copy operators. It helps to keep size of data entangled with this dimension.

    ImageFormatting _format;

	Container _header_data;

    Container _data;
    
};


}
}

