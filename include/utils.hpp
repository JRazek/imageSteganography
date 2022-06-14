#pragma once

#include <cstdint>
#include <cstdio>
#include <iterator>
#include <numeric>
#include <algorithm>
#include <ranges>
#include <bit>
#include <iterator>


namespace jr{



template<
    std::size_t N,
    typename T
>
class vector{

protected:
    std::array<T, N> a;

public:

    using value_type=T;

    constexpr static std::size_t size=N;

    vector(std::array<T, N> const& data): a(data){}

    vector()=default;

    auto operator[](std::size_t const i) noexcept -> T&{
        return a[i];
    }

    auto operator[](std::size_t const i) const noexcept -> T const&{
        return a[i];
    }
    
    auto dim_product() const noexcept -> std::size_t{
        return std::accumulate(a.begin(), a.end(), 1, [](const auto a1, const auto a2){
            return a1*a2;
        });
    }

	auto operator==(vector const& rhs) const -> bool{
		auto mismatch=std::ranges::mismatch(a, rhs.a);
		return a.end()==mismatch.in1;
	}
};

class vector3_size : public jr::vector<3, std::size_t>{
public:
    vector3_size(std::size_t const x, std::size_t const y, std::size_t const z): 
    vector({x, y, z}) 
    {}

    vector3_size()=default;

    auto x() const noexcept -> std::size_t const&{
        return a[0];
    }
    auto x() noexcept -> std::size_t&{
        return a[0];
    }
    auto y() const noexcept -> std::size_t const&{
        return a[1];
    }
    auto y() noexcept -> std::size_t&{
        return a[1];
    }
    auto z() const noexcept -> std::size_t const&{
        return a[2];
    }
    auto z() noexcept -> std::size_t&{
        return a[2];
    }
	
};

template<std::size_t size>
auto to_little_endianness_bytes(std::integral auto n) -> std::vector<std::uint8_t> {
    std::vector<std::uint8_t> bytes(size);

	for(auto& b : bytes){
		b=n&0xFFu;
		n>>=8u;
	}

	if constexpr (std::endian::native==std::endian::little) std::ranges::reverse(bytes);

    return bytes;
}

auto to_little_endianness_bytes(std::integral auto n) -> std::vector<std::uint8_t> { return to_little_endianness_bytes<sizeof(n)>(n); }


template<
std::integral T,
typename Range
>
auto bytes_to_little_endianess(Range range) -> T requires
std::ranges::input_range<Range>
&&
std::ranges::bidirectional_range<Range>
{
	auto n=T(0);

	for(auto const b : range | std::ranges::views::reverse){
		n<<=8u;
		n|=b;
	}
	return n;
}

template<
std::integral T,
std::input_iterator Iterator 
>
auto bytes_to_little_endianess(Iterator low, std::size_t size) -> T {
	auto high=low; std::advance(low, size);
	return bytes_to_little_endianess<T>(std::ranges::subrange{low, high}); 
}

}

