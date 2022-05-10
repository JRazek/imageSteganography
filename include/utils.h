#pragma once

#include <cstdio>
#include <numeric>
#include <algorithm>


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

}