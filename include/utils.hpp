#pragma once

#include <cstdint>
#include <cstdio>
#include <numeric>
#include <algorithm>
#include <ranges>

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

	return bytes;
}

auto to_little_endianness_bytes(std::integral auto n) -> std::vector<std::uint8_t> { return to_little_endianness_bytes<sizeof(n)>(n); }

template<std::integral T>
auto bytes_to_little_endianess(std::bidirectional_iterator auto low, std::bidirectional_iterator auto high) -> T {
	auto n=T(0);

	for(auto const b : std::ranges::subrange{low, high} | std::ranges::views::reverse){
		n<<=8u;
		n|=b;
	}

	return n;
}

template<std::integral T>
auto bytes_to_little_endianess(std::vector<std::uint8_t> const& bytes) -> T { return bytes_to_little_endianess<T>(bytes.cbegin(), bytes.cend()); }
}

