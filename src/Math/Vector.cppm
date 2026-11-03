module;

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <numeric>
#include <ranges>
#include <type_traits>
#include <utility>

export module Vector;

import Number;

export namespace ikk
{
    template<std::size_t N, Number T>
    class [[nodiscard]] Vec final
    {
        static_assert(N >= 1, "Dimension has to be at least 1");
    public:
        inline static constexpr std::size_t Dimension = N;
        using Type = T;

        constexpr Vec() noexcept = default;
        explicit constexpr Vec(T value) noexcept;
        template<class... Args> requires (sizeof...(Args) == N && (Number<std::remove_cvref_t<Args>> && ...) && (std::is_constructible_v<T, Args> && ...))
        constexpr Vec(Args&&... args) noexcept;

        constexpr Vec(const Vec&) noexcept = default;
        constexpr Vec(Vec&&) noexcept = default;

        constexpr Vec& operator=(const Vec&) noexcept = default;
        constexpr Vec& operator=(Vec&&) noexcept = default;

        template<Number U> requires (std::is_convertible_v<U, T>)
        explicit constexpr Vec(const Vec<N, U>& other) noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        explicit constexpr Vec(Vec<N, U>&& other) noexcept;

        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator=(const Vec<N, U>& other) noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator=(Vec<N, U>&& other) noexcept;

        template<std::size_t S>
        explicit constexpr Vec(const Vec<S, T>& other) noexcept;
        template<std::size_t S>
        explicit constexpr Vec(Vec<S, T>&& other) noexcept;

        template<std::size_t S>
        constexpr Vec<N, T>& operator=(const Vec<S, T>& other) noexcept;
        template<std::size_t S>
        constexpr Vec<N, T>& operator=(Vec<S, T>&& other) noexcept;

        template<std::size_t S, Number U> requires (std::is_convertible_v<U, T>)
        explicit constexpr Vec(const Vec<S, U>& other) noexcept;
        template<std::size_t S, Number U> requires (std::is_convertible_v<U, T>)
        explicit constexpr Vec(Vec<S, U>&& other) noexcept;

        template<std::size_t S, Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator=(const Vec<S, U>& other) noexcept;
        template<std::size_t S, Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator=(Vec<S, U>&& other) noexcept;

        constexpr ~Vec() noexcept = default;

        [[nodiscard]] constexpr const std::array<T, N>& data() const noexcept;
        [[nodiscard]] constexpr std::array<T, N>& data() noexcept;

        template<std::size_t index>
        [[nodiscard]] constexpr const T& at() const noexcept;
        template<std::size_t index>
        [[nodiscard]] constexpr T& at() noexcept;

        [[nodiscard]] constexpr const T& operator[](std::size_t index) const noexcept;
        [[nodiscard]] constexpr T& operator[](std::size_t index) noexcept;

        [[nodiscard]] static constexpr Vec X() noexcept requires (N >= 1);
        [[nodiscard]] static constexpr Vec Y() noexcept requires (N >= 2);
        [[nodiscard]] static constexpr Vec Z() noexcept requires (N >= 3);
        [[nodiscard]] static constexpr Vec W() noexcept requires (N >= 4);

        [[nodiscard]] constexpr const T& x() const noexcept requires (N >= 1);
        [[nodiscard]] constexpr const T& y() const noexcept requires (N >= 2);
        [[nodiscard]] constexpr const T& z() const noexcept requires (N >= 3);
        [[nodiscard]] constexpr const T& w() const noexcept requires (N >= 4);

        [[nodiscard]] constexpr T& x() noexcept requires (N >= 1);
        [[nodiscard]] constexpr T& y() noexcept requires (N >= 2);
        [[nodiscard]] constexpr T& z() noexcept requires (N >= 3);
        [[nodiscard]] constexpr T& w() noexcept requires (N >= 4);

        [[nodiscard]] constexpr Vec operator+() const noexcept;
        [[nodiscard]] constexpr Vec operator-() const noexcept;

        [[nodiscard]] constexpr Vec operator+(const Vec& rhs) const noexcept;
        [[nodiscard]] constexpr Vec operator-(const Vec& rhs) const noexcept;
        [[nodiscard]] constexpr Vec operator*(const Vec& rhs) const noexcept;
        [[nodiscard]] constexpr Vec operator/(const Vec& rhs) const noexcept;

        [[nodiscard]] constexpr Vec operator+(T scalar) const noexcept;
        [[nodiscard]] constexpr Vec operator-(T scalar) const noexcept;
        [[nodiscard]] constexpr Vec operator*(T scalar) const noexcept;
        [[nodiscard]] constexpr Vec operator/(T scalar) const noexcept;

        constexpr Vec& operator+=(const Vec& rhs) noexcept;
        constexpr Vec& operator-=(const Vec& rhs) noexcept;
        constexpr Vec& operator*=(const Vec& rhs) noexcept;
        constexpr Vec& operator/=(const Vec& rhs) noexcept;

        constexpr Vec& operator+=(T scalar) noexcept;
        constexpr Vec& operator-=(T scalar) noexcept;
        constexpr Vec& operator*=(T scalar) noexcept;
        constexpr Vec& operator/=(T scalar) noexcept;

        [[nodiscard]] constexpr bool operator==(const Vec& rhs) const noexcept;
        [[nodiscard]] constexpr bool operator!=(const Vec& rhs) const noexcept;
        [[nodiscard]] constexpr bool operator<(const Vec& rhs) const noexcept;
        [[nodiscard]] constexpr bool operator>(const Vec& rhs) const noexcept;
        [[nodiscard]] constexpr bool operator<=(const Vec& rhs) const noexcept;
        [[nodiscard]] constexpr bool operator>=(const Vec& rhs) const noexcept;

        [[nodiscard]] constexpr T dot(const Vec& rhs) const noexcept;
        [[nodiscard]] constexpr T distance(const Vec& rhs) const noexcept;

        [[nodiscard]] constexpr T length_squared() const noexcept;
        [[nodiscard]] constexpr T length() const noexcept;

        constexpr void normalize() noexcept;
        [[nodiscard]] constexpr Vec normalized() const noexcept;

        [[nodiscard]] constexpr T cross(const Vec& rhs) const noexcept requires (N == 2);
        [[nodiscard]] constexpr Vec cross(const Vec& rhs) const noexcept requires (N == 3);

        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr Vec<N, std::common_type_t<T, U>> operator+(const Vec<N, U>& rhs) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr Vec<N, std::common_type_t<T, U>> operator-(const Vec<N, U>& rhs) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr Vec<N, std::common_type_t<T, U>> operator*(const Vec<N, U>& rhs) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr Vec<N, std::common_type_t<T, U>> operator/(const Vec<N, U>& rhs) const noexcept;

        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr Vec<N, std::common_type_t<T, U>> operator+(U scalar) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr Vec<N, std::common_type_t<T, U>> operator-(U scalar) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr Vec<N, std::common_type_t<T, U>> operator*(U scalar) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr Vec<N, std::common_type_t<T, U>> operator/(U scalar) const noexcept;

        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator+=(const Vec<N, U>& rhs) noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator-=(const Vec<N, U>& rhs) noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator*=(const Vec<N, U>& rhs) noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator/=(const Vec<N, U>& rhs) noexcept;

        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator+=(U scalar) noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator-=(U scalar) noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator*=(U scalar) noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        constexpr Vec<N, T>& operator/=(U scalar) noexcept;

        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr bool operator==(const Vec<N, U>& rhs) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr bool operator!=(const Vec<N, U>& rhs) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr bool operator<(const Vec<N, U>& rhs) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr bool operator>(const Vec<N, U>& rhs) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr bool operator<=(const Vec<N, U>& rhs) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr bool operator>=(const Vec<N, U>& rhs) const noexcept;

        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr std::common_type_t<T, U> dot(const Vec<N, U>& rhs) const noexcept;
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr std::common_type_t<T, U> distance(const Vec<N, U>& rhs) const noexcept;

        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr std::common_type_t<T, U> cross(const Vec<N, U>& rhs) const noexcept requires (N == 2);
        template<Number U> requires (std::is_convertible_v<U, T>)
        [[nodiscard]] constexpr Vec<N, std::common_type_t<T, U>> cross(const Vec<N, U>& rhs) const noexcept requires (N == 3);
    private:
        std::array<T, N> m_data{T{}};
    };

    template<Number T, Number... Ts>
    Vec(T, Ts...) -> Vec<1 + sizeof...(Ts), T>;

    template<Number T> using Vec2 = Vec<2, T>;
    template<Number T> using Vec3 = Vec<3, T>;
    template<Number T> using Vec4 = Vec<4, T>;

    using Vec2f = Vec2<float>;
    using Vec2d = Vec2<double>;
    using Vec2i = Vec2<std::int32_t>;
    using Vec2I = Vec2<std::int64_t>;
    using Vec2u = Vec2<std::uint32_t>;
    using Vec2U = Vec2<std::uint64_t>;

    using Vec3f = Vec3<float>;
    using Vec3d = Vec3<double>;
    using Vec3i = Vec3<std::int32_t>;
    using Vec3I = Vec3<std::int64_t>;
    using Vec3u = Vec3<std::uint32_t>;
    using Vec3U = Vec3<std::uint64_t>;

    using Vec4f = Vec4<float>;
    using Vec4d = Vec4<double>;
    using Vec4i = Vec4<std::int32_t>;
    using Vec4I = Vec4<std::int64_t>;
    using Vec4u = Vec4<std::uint32_t>;
    using Vec4U = Vec4<std::uint64_t>;

    template<std::size_t N, Number T>
    [[nodiscard]] inline constexpr Vec<N, T> operator+(T scalar, const Vec<N, T>& v) noexcept
    {
        return v + scalar;
    }

    template<std::size_t N, Number T>
    [[nodiscard]] inline constexpr Vec<N, T> operator-(T scalar, const Vec<N, T>& v) noexcept
    {
        return v - scalar;
    }

    template<std::size_t N, Number T>
    [[nodiscard]] inline constexpr Vec<N, T> operator*(T scalar, const Vec<N, T>& v) noexcept
    {
        return v * scalar;
    }

    template<std::size_t N, Number T>
    [[nodiscard]] inline constexpr Vec<N, T> operator/(T scalar, const Vec<N, T>& v) noexcept
    {
        return v / scalar;
    }

    template<std::size_t N, Number T, Number U> requires (std::is_convertible_v<U, T>)
    [[nodiscard]] inline constexpr Vec<N, U> operator+(T scalar, const Vec<N, U>& v) noexcept
    {
        return v + scalar;
    }

    template<std::size_t N, Number T, Number U> requires (std::is_convertible_v<U, T>)
    [[nodiscard]] inline constexpr Vec<N, U> operator-(T scalar, const Vec<N, U>& v) noexcept
    {
        return v - scalar;
    }

    template<std::size_t N, Number T, Number U> requires (std::is_convertible_v<U, T>)
    [[nodiscard]] inline constexpr Vec<N, U> operator*(T scalar, const Vec<N, U>& v) noexcept
    {
        return v * scalar;
    }

    template<std::size_t N, Number T, Number U> requires (std::is_convertible_v<U, T>)
    [[nodiscard]] inline constexpr Vec<N, U> operator/(T scalar, const Vec<N, U>& v) noexcept
    {
        return v / scalar;
    }
}

namespace ikk
{
    template<std::size_t N, Number T>
    constexpr Vec<N, T>::Vec(T value) noexcept
    {
        std::ranges::fill(this->m_data, value);
    }

    template<std::size_t N, Number T>
    template<class... Args> requires (sizeof...(Args) == N && (Number<std::remove_cvref_t<Args>> && ...) && (std::is_constructible_v<T, Args> && ...))
    constexpr Vec<N, T>::Vec(Args&&... args) noexcept
        : m_data{static_cast<T>(std::forward<Args>(args))...}
    {
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>::Vec(const Vec<N, U>& other) noexcept
    {
        std::ranges::copy(other.data().begin(), other.data().end(), this->m_data.begin());
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>::Vec(Vec<N, U>&& other) noexcept
    {
        std::ranges::move(other.data().begin(), other.data().end(), this->m_data.begin());
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator=(const Vec<N, U>& other) noexcept
    {
        std::ranges::copy(other.data().begin(), other.data().end(), this->m_data.begin());
        return *this;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator=(Vec<N, U>&& other) noexcept
    {
        std::ranges::move(other.data().begin(), other.data().end(), this->m_data.begin());
        return *this;
    }

    template<std::size_t N, Number T>
    template<std::size_t S>
    constexpr Vec<N, T>::Vec(const Vec<S, T>& other) noexcept
    {
        std::ranges::copy_n(other.data().begin(), std::min(N, S), this->m_data.begin());
    }

    template<std::size_t N, Number T>
    template<std::size_t S>
    constexpr Vec<N, T>::Vec(Vec<S, T>&& other) noexcept
    {
        std::ranges::move(other.data().begin(), other.data().begin() + std::min(N, S), this->m_data.begin());
    }

    template<std::size_t N, Number T>
    template<std::size_t S>
    constexpr Vec<N, T>& Vec<N, T>::operator=(const Vec<S, T>& other) noexcept
    {
        std::ranges::copy_n(other.data().begin(), std::min(N, S), this->m_data.begin());
        return *this;
    }

    template<std::size_t N, Number T>
    template<std::size_t S>
    constexpr Vec<N, T>& Vec<N, T>::operator=(Vec<S, T>&& other) noexcept
    {
        std::ranges::move(other.data().begin(), other.data().begin() + std::min(N, S), this->m_data.begin());
        return *this;
    }

    template<std::size_t N, Number T>
    template<std::size_t S, Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>::Vec(const Vec<S, U>& other) noexcept
    {
        std::ranges::transform(other.data().begin(), other.data().begin() + std::min(N, S), this->m_data.begin(),
            [](const U& value) noexcept
            {
                return static_cast<T>(value);
            });
    }

    template<std::size_t N, Number T>
    template<std::size_t S, Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>::Vec(Vec<S, U>&& other) noexcept
    {
        std::ranges::transform(other.data().begin(), other.data().begin() + std::min(N, S), this->m_data.begin(),
            [](U& value) noexcept
            {
                return static_cast<T>(std::move(value));
            });
    }

    template<std::size_t N, Number T>
    template<std::size_t S, Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator=(const Vec<S, U>& other) noexcept
    {
        std::ranges::transform(other.data().begin(), other.data().begin() + std::min(N, S), this->m_data.begin(),
            [](const U& value) noexcept
            {
                return static_cast<T>(value);
            });
        return *this;
    }

    template<std::size_t N, Number T>
    template<std::size_t S, Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator=(Vec<S, U>&& other) noexcept
    {
        std::ranges::transform(other.data().begin(), other.data().begin() + std::min(N, S), this->m_data.begin(),
            [](U& value) noexcept
            {
                return static_cast<T>(std::move(value));
            });
        return *this;
    }

    template<std::size_t N, Number T>
    constexpr const std::array<T, N>& Vec<N, T>::data() const noexcept
    {
        return this->m_data;
    }

    template<std::size_t N, Number T>
    constexpr std::array<T, N>& Vec<N, T>::data() noexcept
    {
        return this->m_data;
    }

    template<std::size_t N, Number T>
    template<std::size_t index>
    constexpr const T& Vec<N, T>::at() const noexcept
    {
        static_assert(index < N);
        return this->m_data[index];
    }

    template<std::size_t N, Number T>
    template<std::size_t index>
    constexpr T& Vec<N, T>::at() noexcept
    {
        static_assert(index < N);
        return this->m_data[index];
    }

    template<std::size_t N, Number T>
    constexpr const T& Vec<N, T>::operator[](std::size_t index) const noexcept
    {
        assert(index < N);
        return this->m_data[index];
    }

    template<std::size_t N, Number T>
    constexpr T& Vec<N, T>::operator[](std::size_t index) noexcept
    {
        assert(index < N);
        return this->m_data[index];
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::X() noexcept requires (N >= 1)
    {
        Vec<N, T> v{};
        v.x() = T{1};
        return v;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::Y() noexcept requires (N >= 2)
    {
        Vec<N, T> v{};
        v.y() = T{1};
        return v;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::Z() noexcept requires (N >= 3)
    {
        Vec<N, T> v{};
        v.z() = T{1};
        return v;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::W() noexcept requires (N >= 4)
    {
        Vec<N, T> v{};
        v.w() = T{1};
        return v;
    }

    template<std::size_t N, Number T>
    constexpr const T& Vec<N, T>::x() const noexcept requires (N >= 1)
    {
        return m_data[0];
    }

    template<std::size_t N, Number T>
    constexpr const T& Vec<N, T>::y() const noexcept requires (N >= 2)
    {
        return m_data[1];
    }

    template<std::size_t N, Number T>
    constexpr const T& Vec<N, T>::z() const noexcept requires (N >= 3)
    {
        return m_data[2];
    }

    template<std::size_t N, Number T>
    constexpr const T& Vec<N, T>::w() const noexcept requires (N >= 4)
    {
        return m_data[3];
    }

    template<std::size_t N, Number T>
    constexpr T& Vec<N, T>::x() noexcept requires (N >= 1)
    {
        return m_data[0];
    }

    template<std::size_t N, Number T>
    constexpr T& Vec<N, T>::y() noexcept requires (N >= 2)
    {
        return m_data[1];
    }

    template<std::size_t N, Number T>
    constexpr T& Vec<N, T>::z() noexcept requires (N >= 3)
    {
        return m_data[2];
    }

    template<std::size_t N, Number T>
    constexpr T& Vec<N, T>::w() noexcept requires (N >= 4)
    {
        return m_data[3];
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator+() const noexcept
    {
        return Vec<N, T>{*this};
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator-() const noexcept
    {
        Vec<N, T> out{};
        std::ranges::transform(this->m_data, out.m_data.begin(), std::negate<>{});
        return out;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator+(const Vec& rhs) const noexcept
    {
        Vec<N, T> out = *this;
        out += rhs;
        return out;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator-(const Vec& rhs) const noexcept
    {
        Vec<N, T> out = *this;
        out -= rhs;
        return out;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator*(const Vec& rhs) const noexcept
    {
        Vec<N, T> out = *this;
        out *= rhs;
        return out;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator/(const Vec& rhs) const noexcept
    {
        Vec<N, T> out = *this;
        out /= rhs;
        return out;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator+(T scalar) const noexcept
    {
        Vec<N, T> out = *this;
        out += scalar;
        return out;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator-(T scalar) const noexcept
    {
        Vec<N, T> out = *this;
        out -= scalar;
        return out;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator*(T scalar) const noexcept
    {
        Vec<N, T> out = *this;
        out *= scalar;
        return out;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::operator/(T scalar) const noexcept
    {
        Vec<N, T> out = *this;
        out /= scalar;
        return out;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T>& Vec<N, T>::operator+=(const Vec& rhs) noexcept
    {
        std::ranges::transform(this->m_data, rhs.m_data, this->m_data.begin(), std::plus<>{});
        return *this;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T>& Vec<N, T>::operator-=(const Vec& rhs) noexcept
    {
        std::ranges::transform(this->m_data, rhs.m_data, this->m_data.begin(), std::minus<>{});
        return *this;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T>& Vec<N, T>::operator*=(const Vec& rhs) noexcept
    {
        std::ranges::transform(this->m_data, rhs.m_data, this->m_data.begin(), std::multiplies<>{});
        return *this;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T>& Vec<N, T>::operator/=(const Vec& rhs) noexcept
    {
        std::ranges::transform(this->m_data, rhs.m_data, this->m_data.begin(), std::divides<>{});
        return *this;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T>& Vec<N, T>::operator+=(T scalar) noexcept
    {
        std::ranges::transform(this->m_data, this->m_data.begin(), std::bind_back(std::plus<>{}, scalar));
        return *this;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T>& Vec<N, T>::operator-=(T scalar) noexcept
    {
        std::ranges::transform(this->m_data, this->m_data.begin(), std::bind_back(std::minus<>{}, scalar));
        return *this;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T>& Vec<N, T>::operator*=(T scalar) noexcept
    {
        std::ranges::transform(this->m_data, this->m_data.begin(), std::bind_back(std::multiplies<>{}, scalar));
        return *this;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T>& Vec<N, T>::operator/=(T scalar) noexcept
    {
        std::ranges::transform(this->m_data, this->m_data.begin(), std::bind_back(std::divides<>{}, scalar));
        return *this;
    }

    template<std::size_t N, Number T>
    constexpr bool Vec<N, T>::operator==(const Vec& rhs) const noexcept
    {
        return std::ranges::equal(this->m_data, rhs.m_data);
    }

    template<std::size_t N, Number T>
    constexpr bool Vec<N, T>::operator!=(const Vec& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    template<std::size_t N, Number T>
    constexpr bool Vec<N, T>::operator<(const Vec& rhs) const noexcept
    {
        return this->length_squared() < rhs.length_squared();
    }

    template<std::size_t N, Number T>
    constexpr bool Vec<N, T>::operator>(const Vec& rhs) const noexcept
    {
        return this->length_squared() > rhs.length_squared();
    }

    template<std::size_t N, Number T>
    constexpr bool Vec<N, T>::operator<=(const Vec& rhs) const noexcept
    {
        return this->length_squared() <= rhs.length_squared();
    }

    template<std::size_t N, Number T>
    constexpr bool Vec<N, T>::operator>=(const Vec& rhs) const noexcept
    {
        return this->length_squared() >= rhs.length_squared();
    }

    template<std::size_t N, Number T>
    constexpr T Vec<N, T>::dot(const Vec& rhs) const noexcept
    {
        return std::inner_product(this->m_data.begin(), this->m_data.end(), rhs.data().begin(), T{0});
    }

    template<std::size_t N, Number T>
    constexpr T Vec<N, T>::distance(const Vec& rhs) const noexcept
    {
        const Vec<N, T> diff = *this - rhs;
        return diff.length();
    }

    template<std::size_t N, Number T>
    constexpr T Vec<N, T>::length_squared() const noexcept
    {
        return this->dot(*this);
    }

    template<std::size_t N, Number T>
    constexpr T Vec<N, T>::length() const noexcept
    {
        return std::sqrt(this->length_squared());
    }

    template<std::size_t N, Number T>
    constexpr void Vec<N, T>::normalize() noexcept
    {
        const T len = this->length();
        if (isZero(len) == false)
            *this /= len;
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::normalized() const noexcept
    {
        Vec<N, T> out = *this;
        out.normalize();
        return out;
    }

    template<std::size_t N, Number T>
    constexpr T Vec<N, T>::cross(const Vec& rhs) const noexcept requires (N == 2)
    {
        return (this->x() * rhs.y()) - (this->y() * rhs.x());
    }

    template<std::size_t N, Number T>
    constexpr Vec<N, T> Vec<N, T>::cross(const Vec& rhs) const noexcept requires (N == 3)
    {
        return Vec<N, T>
            {
                (this->y() * rhs.z()) - (this->z() * rhs.y()),
                (this->z() * rhs.x()) - (this->x() * rhs.z()),
                (this->x() * rhs.y()) - (this->y() * rhs.x())
            };
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, std::common_type_t<T, U>> Vec<N, T>::operator+(const Vec<N, U>& rhs) const noexcept
    {
        Vec<N, std::common_type_t<T, U>> out = *this;
        out += rhs;
        return out;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, std::common_type_t<T, U>> Vec<N, T>::operator-(const Vec<N, U>& rhs) const noexcept
    {
        Vec<N, std::common_type_t<T, U>> out = *this;
        out -= rhs;
        return out;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, std::common_type_t<T, U>> Vec<N, T>::operator*(const Vec<N, U>& rhs) const noexcept
    {
        Vec<N, std::common_type_t<T, U>> out = *this;
        out *= rhs;
        return out;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, std::common_type_t<T, U>> Vec<N, T>::operator/(const Vec<N, U>& rhs) const noexcept
    {
        Vec<N, std::common_type_t<T, U>> out = *this;
        out /= rhs;
        return out;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, std::common_type_t<T, U>> Vec<N, T>::operator+(U scalar) const noexcept
    {
        Vec<N, std::common_type_t<T, U>> out = *this;
        out += static_cast<std::common_type_t<T, U>>(scalar);
        return out;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, std::common_type_t<T, U>> Vec<N, T>::operator-(U scalar) const noexcept
    {
        Vec<N, std::common_type_t<T, U>> out = *this;
        out -= static_cast<std::common_type_t<T, U>>(scalar);
        return out;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, std::common_type_t<T, U>> Vec<N, T>::operator*(U scalar) const noexcept
    {
        Vec<N, std::common_type_t<T, U>> out = *this;
        out *= static_cast<std::common_type_t<T, U>>(scalar);
        return out;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, std::common_type_t<T, U>> Vec<N, T>::operator/(U scalar) const noexcept
    {
        Vec<N, std::common_type_t<T, U>> out = *this;
        out /= static_cast<std::common_type_t<T, U>>(scalar);
        return out;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator+=(const Vec<N, U>& rhs) noexcept
    {
        for (std::size_t i = 0; i < N; ++i) this->m_data[i] += static_cast<T>(rhs[i]);
        return *this;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator-=(const Vec<N, U>& rhs) noexcept
    {
        for (std::size_t i = 0; i < N; ++i) this->m_data[i] -= static_cast<T>(rhs[i]);
        return *this;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator*=(const Vec<N, U>& rhs) noexcept
    {
        for (std::size_t i = 0; i < N; ++i) this->m_data[i] *= static_cast<T>(rhs[i]);
        return *this;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator/=(const Vec<N, U>& rhs) noexcept
    {
        for (std::size_t i = 0; i < N; ++i) this->m_data[i] /= static_cast<T>(rhs[i]);
        return *this;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator+=(U scalar) noexcept
    {
        for (std::size_t i = 0; i < N; ++i) this->m_data[i] += static_cast<T>(scalar);
        return *this;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator-=(U scalar) noexcept
    {
        for (std::size_t i = 0; i < N; ++i) this->m_data[i] -= static_cast<T>(scalar);
        return *this;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator*=(U scalar) noexcept
    {
        for (std::size_t i = 0; i < N; ++i) this->m_data[i] *= static_cast<T>(scalar);
        return *this;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, T>& Vec<N, T>::operator/=(U scalar) noexcept
    {
        for (std::size_t i = 0; i < N; ++i) this->m_data[i] /= static_cast<T>(scalar);
        return *this;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr bool Vec<N, T>::operator==(const Vec<N, U>& rhs) const noexcept
    {
        for (std::size_t i = 0; i < N; ++i) if (this->m_data[i] != static_cast<T>(rhs[i])) return false;
        return true;
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr bool Vec<N, T>::operator!=(const Vec<N, U>& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr bool Vec<N, T>::operator<(const Vec<N, U>& rhs) const noexcept
    {
        return this->length_squared() < static_cast<T>(rhs.length_squared());
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr bool Vec<N, T>::operator>(const Vec<N, U>& rhs) const noexcept
    {
        return this->length_squared() > static_cast<T>(rhs.length_squared());
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr bool Vec<N, T>::operator<=(const Vec<N, U>& rhs) const noexcept
    {
        return this->length_squared() <= static_cast<T>(rhs.length_squared());
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr bool Vec<N, T>::operator>=(const Vec<N, U>& rhs) const noexcept
    {
        return this->length_squared() >= static_cast<T>(rhs.length_squared());
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr std::common_type_t<T, U> Vec<N, T>::dot(const Vec<N, U>& rhs) const noexcept
    {
        return std::inner_product(this->m_data.begin(), this->m_data.end(), rhs.data().begin(), std::common_type_t<T, U>{0});
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr std::common_type_t<T, U> Vec<N, T>::distance(const Vec<N, U>& rhs) const noexcept
    {
        const Vec<N, std::common_type_t<T, U>> diff = *this - rhs;
        return diff.length();
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr std::common_type_t<T, U> Vec<N, T>::cross(const Vec<N, U>& rhs) const noexcept requires (N == 2)
    {
        return static_cast<std::common_type_t<T, U>>((this->x() * static_cast<T>(rhs.y())) - (this->y() * static_cast<T>(rhs.x())));
    }

    template<std::size_t N, Number T>
    template<Number U> requires (std::is_convertible_v<U, T>)
    constexpr Vec<N, std::common_type_t<T, U>> Vec<N, T>::cross(const Vec<N, U>& rhs) const noexcept requires (N == 3)
    {
        return Vec<N, std::common_type_t<T, U>>
        {
            static_cast<std::common_type_t<T, U>>((this->y() * static_cast<T>(rhs.z())) - (this->z() * static_cast<T>(rhs.y()))),
            static_cast<std::common_type_t<T, U>>((this->z() * static_cast<T>(rhs.x())) - (this->x() * static_cast<T>(rhs.z()))),
            static_cast<std::common_type_t<T, U>>((this->x() * static_cast<T>(rhs.y())) - (this->y() * static_cast<T>(rhs.x())))
        };
    }
}