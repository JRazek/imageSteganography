#include <bitmap.hpp>
#include <algorithm>
#include <numeric>
#include <cassert>


namespace jr{
namespace img{



Bitmap::Bitmap(Bitmap const& bmp) noexcept = default;

Bitmap::Bitmap(Bitmap&& bmp) noexcept = default;

Bitmap::Bitmap() noexcept = default;

Bitmap::Bitmap(jr::vector3_size const size) noexcept: 
_size(size),
_data(size.dim_product())
{}

Bitmap::Bitmap(std::size_t const w, std::size_t const h, std::size_t const d) noexcept:
Bitmap(jr::vector3_size{w, h, d})
{}

auto Bitmap::set_format(ImageFormatting const format) noexcept -> void{
    _format=format;
}

auto Bitmap::format() const noexcept -> ImageFormatting{
    return _format;
}

auto Bitmap::set_data(Container const& data) noexcept -> void{
    assert(data.size()==_size.dim_product());
    _data=data;
}

auto Bitmap::set_data(Container&& data) noexcept -> void{
    assert(data.size()==_size.dim_product());
    _data=std::move(data);
}

auto Bitmap::size() const noexcept -> jr::vector3_size{
	return _size;
}


auto Bitmap::get(jr::vector3_size const vec) noexcept -> data_type{
    return _data[get_index(vec)];
}


auto Bitmap::get_ref(jr::vector3_size const vec) noexcept -> data_type&{
    return _data[get_index(vec)];
}

auto Bitmap::get_ref(jr::vector3_size const vec) const noexcept -> data_type const&{
    return _data[get_index(vec)];
}

auto Bitmap::begin() const noexcept -> Container::const_iterator{ return _data.cbegin(); }

auto Bitmap::end() const noexcept -> Container::const_iterator{ return _data.cend(); }

auto Bitmap::begin() noexcept -> Container::iterator{ return _data.begin(); }

auto Bitmap::end() noexcept -> Container::iterator{ return _data.end(); }

auto Bitmap::get_index(jr::vector3_size const vec) const noexcept -> std::size_t{
    assert(vec.x()>=0 && vec.x()<_size.x() && vec.y()>=0 && vec.y()<_size.y() && vec.z()>=0 && vec.z()<_size.z());

    return vec.y()*_size.z()*_size.x() + vec.x()*_size.z() + vec.z();
}

auto Bitmap::getVector(std::size_t const index) const noexcept -> jr::vector3_size{
    assert(index < _size.dim_product());

    const auto x=(index%_size.y())/_size.x();
    const auto y=index/(_size.z()*_size.x());
    const auto z=index%_size.z();
    return {x, y, z};
}

auto Bitmap::operator=(Bitmap const& bmp) noexcept -> Bitmap& = default;

auto Bitmap::operator=(Bitmap&& bmp) noexcept -> Bitmap& = default;

auto Bitmap::operator==(Bitmap const& bmp) noexcept -> bool{
	if(_size!=bmp.size()) return false;
	auto mismatch=std::ranges::mismatch(_data, bmp._data);
	return _data.end()==mismatch.in1;
}


}
}

