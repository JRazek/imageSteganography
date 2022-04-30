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

    auto operator[](std::size_t i) noexcept -> T&{
        return a[i];
    }

    auto operator[](std::size_t i) const noexcept -> T const&{
        return a[i];
    }
    
    auto dim_product() const noexcept -> std::size_t{
        return std::accumulate(a.begin(), a.end(), 1, [](const auto a1, const auto a2){
            return a1*a2;
        });
    }
};

class vector3_size : public vector<3, std::size_t>{
public:
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