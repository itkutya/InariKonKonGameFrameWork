module;

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <functional>
#include <ranges>

export module Matrix;

import Number;
import Vector;

export namespace ikk
{
    enum struct [[nodiscard]] MatrixOrdering : std::uint8_t
    {
        RowMajor, ColumnMajor
    };

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O = MatrixOrdering::RowMajor>
    class [[nodiscard]] Mat final
    {
        static_assert(Rows >= 1 && Cols >= 1, "Matrix must have at least one row and column");
    public:
        enum struct Axis : std::uint8_t
        {
            Row, Column
        };

        template<Axis Axis>
        class [[nodiscard]] VecView final
        {
        public:
            inline static constexpr std::size_t Size = (Axis == Axis::Row) ? Rows : Cols;

            [[nodiscard]] inline constexpr VecView(Mat& matrix, std::size_t index) noexcept
                : m_matrix(&matrix), m_index(index)
            {
            }

            constexpr VecView(const VecView&) noexcept = delete;
            constexpr VecView(VecView&&) noexcept = default;

            constexpr VecView& operator=(const VecView&) noexcept = delete;
            constexpr VecView& operator=(VecView&&) noexcept = default;

            constexpr ~VecView() noexcept = default;

            inline constexpr const T& operator[](std::size_t i) const noexcept
            {
                return this->element(i);
            }

            inline constexpr T& operator[](std::size_t i) noexcept
            {
                return this->element(i);
            }

            inline constexpr operator Vec<Size, T>() const noexcept
            {
                Vec<Size, T> out{};
                for (std::size_t i = 0; i < Size; ++i)
                    out[i] = this->element(i);
                return out;
            }

            inline constexpr VecView& operator=(const Vec<Size, T>& vec) noexcept
            {
                for (std::size_t i = 0; i < Size; ++i)
                    this->element(i) = vec[i];
                return *this;
            }
        private:
            Mat* m_matrix = nullptr;
            std::size_t m_index = 0;

            inline constexpr T& element(std::size_t i) const noexcept
            {
                assert(i < Size);
                if constexpr (Axis == Axis::Row)
                    return (*this->m_matrix)[this->m_index, i];
                else
                    return (*this->m_matrix)[i, this->m_index];
            }
        };

        using Type = T;

        constexpr Mat() noexcept = default;
        explicit constexpr Mat(T value) noexcept;
        template<class... Args> requires (sizeof...(Args) == Rows * Cols && (Number<std::remove_cvref_t<Args>> && ...) && (std::is_constructible_v<T, Args> && ...))
        constexpr Mat(Args&&... args) noexcept;

        constexpr Mat(const Mat&) noexcept = default;
        constexpr Mat(Mat&&) noexcept = default;

        constexpr Mat& operator=(const Mat&) noexcept = default;
        constexpr Mat& operator=(Mat&&) noexcept = default;

        template<MatrixOrdering U> requires (U != O)
        explicit constexpr Mat(const Mat<Rows, Cols, T, U>& other) noexcept;
        template<MatrixOrdering U> requires (U != O)
        explicit constexpr Mat(Mat<Rows, Cols, T, U>&& other) noexcept;

        template<MatrixOrdering U> requires (U != O)
        constexpr Mat& operator=(const Mat<Rows, Cols, T, U>& other) noexcept;
        template<MatrixOrdering U> requires (U != O)
        constexpr Mat& operator=(Mat<Rows, Cols, T, U>&& other) noexcept;

        constexpr ~Mat() noexcept = default;

        [[nodiscard]] static constexpr Mat Identity() noexcept requires (Rows == Cols);

        [[nodiscard]] constexpr const T& operator[](std::size_t row, std::size_t column) const noexcept;
        [[nodiscard]] constexpr T& operator[](std::size_t row, std::size_t column) noexcept;

        template<std::size_t RowIndex, std::size_t ColumnIndex>
        [[nodiscard]] constexpr const T& at() const noexcept;
        template<std::size_t RowIndex, std::size_t ColumnIndex>
        [[nodiscard]] constexpr T& at() noexcept;

        [[nodiscard]] constexpr const std::array<T, Rows * Cols>& data() const noexcept;
        [[nodiscard]] constexpr std::array<T, Rows * Cols>& data() noexcept;

        [[nodiscard]] constexpr Mat operator+(const Mat& rhs) const noexcept;
        [[nodiscard]] constexpr Mat operator-(const Mat& rhs) const noexcept;
        [[nodiscard]] constexpr Mat operator*(const Mat& rhs) const noexcept requires (Rows == Cols);
        [[nodiscard]] constexpr Mat operator/(const Mat& rhs) const noexcept requires (Rows == Cols);

        constexpr Mat& operator+=(const Mat& rhs) noexcept;
        constexpr Mat& operator-=(const Mat& rhs) noexcept;
        constexpr Mat& operator*=(const Mat& rhs) noexcept requires (Rows == Cols);
        constexpr Mat& operator/=(const Mat& rhs) noexcept requires (Rows == Cols);

        [[nodiscard]] constexpr Mat operator+(T scalar) const noexcept;
        [[nodiscard]] constexpr Mat operator-(T scalar) const noexcept;
        [[nodiscard]] constexpr Mat operator*(T scalar) const noexcept;
        [[nodiscard]] constexpr Mat operator/(T scalar) const noexcept;

        constexpr Mat& operator+=(T scalar) noexcept;
        constexpr Mat& operator-=(T scalar) noexcept;
        constexpr Mat& operator*=(T scalar) noexcept;
        constexpr Mat& operator/=(T scalar) noexcept;

        [[nodiscard]] constexpr Vec<Rows, T> operator*(const Vec<Cols, T>& vec) const noexcept;

        [[nodiscard]] constexpr bool operator==(const Mat&) const noexcept = default;
        [[nodiscard]] constexpr bool operator!=(const Mat&) const noexcept = default;

        constexpr void transpose() noexcept requires (Rows == Cols);
        constexpr void invert() noexcept requires (Rows == Cols);

        [[nodiscard]] constexpr Mat transposed() const noexcept requires (Rows == Cols);
        [[nodiscard]] constexpr Mat inverted() const noexcept requires (Rows == Cols);

        [[nodiscard]] constexpr VecView<Axis::Row> row(std::size_t row) const noexcept;
        [[nodiscard]] constexpr VecView<Axis::Row> row(std::size_t row) noexcept;

        [[nodiscard]] constexpr VecView<Axis::Column> column(std::size_t column) const noexcept;
        [[nodiscard]] constexpr VecView<Axis::Column> column(std::size_t column) noexcept;

        template<class Func> requires (std::invocable<Func&, T&> && std::same_as<std::invoke_result_t<Func&, T&>, void>)
        constexpr void row(std::size_t row, Func&& func) noexcept;

        template<class Func> requires (std::invocable<Func&, T&> && std::same_as<std::invoke_result_t<Func&, T&>, void>)
        constexpr void column(std::size_t column, Func&& func) noexcept;
    private:
        std::array<T, Rows * Cols> m_data{};

        [[nodiscard]] static constexpr std::size_t index(std::size_t row, std::size_t col) noexcept;
    };

    template<Number T> using Mat3x3 = Mat<3, 3, T>;
    template<Number T> using Mat4x4 = Mat<4, 4, T>;

    using Mat3x3f = Mat3x3<float>;
    using Mat3x3d = Mat3x3<double>;
    using Mat3x3i = Mat3x3<std::int32_t>;
    using Mat3x3I = Mat3x3<std::int64_t>;
    using Mat3x3u = Mat3x3<std::uint32_t>;
    using Mat3x3U = Mat3x3<std::uint64_t>;

    using Mat4x4f = Mat4x4<float>;
    using Mat4x4d = Mat4x4<double>;
    using Mat4x4i = Mat4x4<std::int32_t>;
    using Mat4x4I = Mat4x4<std::int64_t>;
    using Mat4x4u = Mat4x4<std::uint32_t>;
    using Mat4x4U = Mat4x4<std::uint64_t>;

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O = MatrixOrdering::RowMajor>
    [[nodiscard]] inline constexpr Mat<Rows, Cols, T, O> operator+(T scalar, const Mat<Rows, Cols, T, O>& mat) noexcept
    {
        return mat + scalar;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O = MatrixOrdering::RowMajor>
    [[nodiscard]] inline constexpr Mat<Rows, Cols, T, O> operator-(T scalar, const Mat<Rows, Cols, T, O>& mat) noexcept
    {
        return mat - scalar;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O = MatrixOrdering::RowMajor>
    [[nodiscard]] inline constexpr Mat<Rows, Cols, T, O> operator*(T scalar, const Mat<Rows, Cols, T, O>& mat) noexcept
    {
        return mat * scalar;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O = MatrixOrdering::RowMajor>
    [[nodiscard]] inline constexpr Mat<Rows, Cols, T, O> operator/(T scalar, const Mat<Rows, Cols, T, O>& mat) noexcept
    {
        return mat / scalar;
    }
}

namespace ikk
{
    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>::Mat(T value) noexcept
    {
        for (std::size_t row = 0; row < Rows; ++row)
            for (std::size_t col = 0; col < Cols; ++col)
                if (row == col)
                    this->m_data[row * Cols + col] = value;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    template<class ... Args> requires (sizeof...(Args) == Rows * Cols && (Number<std::remove_cvref_t<Args>> && ...) && (std::is_constructible_v<T, Args> && ...))
    constexpr Mat<Rows, Cols, T, O>::Mat(Args&&... args) noexcept
        : m_data{static_cast<T>(std::forward<Args>(args))...}
    {
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    template<MatrixOrdering U> requires (U != O)
    constexpr Mat<Rows, Cols, T, O>::Mat(const Mat<Rows, Cols, T, U>& other) noexcept
    {
        for (auto [r, c] : std::views::cartesian_product(
            std::views::iota(std::size_t{0}, Rows),
            std::views::iota(std::size_t{0}, Cols)))
            (*this)[r, c] = other[r, c];
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    template<MatrixOrdering U> requires (U != O)
    constexpr Mat<Rows, Cols, T, O>::Mat(Mat<Rows, Cols, T, U>&& other) noexcept
    {
        for (auto [r, c] : std::views::cartesian_product(
            std::views::iota(std::size_t{0}, Rows),
            std::views::iota(std::size_t{0}, Cols)))
            (*this)[r, c] = other[r, c];
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    template<MatrixOrdering U> requires (U != O)
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator=(const Mat<Rows, Cols, T, U>& other) noexcept
    {
        for (auto [r, c] : std::views::cartesian_product(
            std::views::iota(std::size_t{0}, Rows),
            std::views::iota(std::size_t{0}, Cols)))
            (*this)[r, c] = other[r, c];
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    template<MatrixOrdering U> requires (U != O)
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator=(Mat<Rows, Cols, T, U>&& other) noexcept
    {
        for (auto [r, c] : std::views::cartesian_product(
            std::views::iota(std::size_t{0}, Rows),
            std::views::iota(std::size_t{0}, Cols)))
            (*this)[r, c] = other[r, c];
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::Identity() noexcept requires (Rows == Cols)
    {
        Mat<Rows, Cols, T, O> out{};
        std::ranges::fill(out.m_data | std::views::stride(Rows + 1), T{1});
        return out;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr const T& Mat<Rows, Cols, T, O>::operator[](std::size_t row, std::size_t column) const noexcept
    {
        return this->m_data[index(row, column)];
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr T& Mat<Rows, Cols, T, O>::operator[](std::size_t row, std::size_t column) noexcept
    {
        return this->m_data[index(row, column)];
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    template<std::size_t RowIndex, std::size_t ColumnIndex>
    constexpr const T& Mat<Rows, Cols, T, O>::at() const noexcept
    {
        static_assert(RowIndex < Rows && ColumnIndex < Cols, "Index out of bounds");
        return this->m_data[index(RowIndex, ColumnIndex)];
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    template<std::size_t RowIndex, std::size_t ColumnIndex>
    constexpr T& Mat<Rows, Cols, T, O>::at() noexcept
    {
        static_assert(RowIndex < Rows && ColumnIndex < Cols, "Index out of bounds");
        return this->m_data[index(RowIndex, ColumnIndex)];
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr const std::array<T, Rows * Cols>& Mat<Rows, Cols, T, O>::data() const noexcept
    {
        return this->m_data;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr std::array<T, Rows * Cols>& Mat<Rows, Cols, T, O>::data() noexcept
    {
        return this->m_data;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::operator+(const Mat& rhs) const noexcept
    {
        Mat<Rows, Cols, T, O> result = *this;
        result += rhs;
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::operator-(const Mat& rhs) const noexcept
    {
        Mat<Rows, Cols, T, O> result = *this;
        result -= rhs;
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::operator*(const Mat& rhs) const noexcept requires (Rows == Cols)
    {
        Mat<Rows, Cols, T, O> result = *this;
        result *= rhs;
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::operator/(const Mat& rhs) const noexcept requires (Rows == Cols)
    {
        Mat<Rows, Cols, T, O> result = *this;
        result /= rhs;
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator+=(const Mat& rhs) noexcept
    {
        std::ranges::transform(this->m_data, rhs.m_data, this->m_data.begin(), std::plus<>{});
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator-=(const Mat& rhs) noexcept
    {
        std::ranges::transform(this->m_data, rhs.m_data, this->m_data.begin(), std::minus<>{});
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator*=(const Mat& rhs) noexcept requires (Rows == Cols)
    {
        Mat<Rows, Cols, T, O> lhs = *this;
        for (std::size_t i = 0; i < Rows; ++i)
            for (std::size_t j = 0; j < Cols; ++j)
            {
                T sum{};
                for (std::size_t k = 0; k < Cols; ++k)
                    sum += lhs[i, k] * rhs[k, j];
                (*this)[i, j] = sum;
            }
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator/=(const Mat& rhs) noexcept requires (Rows == Cols)
    {
        Mat<Rows, Cols, T, O> inv = rhs;
        inv.invert();
        (*this) *= inv;
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::operator+(T scalar) const noexcept
    {
        Mat<Rows, Cols, T, O> result = *this;
        result += scalar;
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::operator-(T scalar) const noexcept
    {
        Mat<Rows, Cols, T, O> result = *this;
        result -= scalar;
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::operator*(T scalar) const noexcept
    {
        Mat<Rows, Cols, T, O> result = *this;
        result *= scalar;
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::operator/(T scalar) const noexcept
    {
        Mat<Rows, Cols, T, O> result = *this;
        result /= scalar;
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator+=(T scalar) noexcept
    {
        std::ranges::transform(this->m_data, this->m_data.begin(), std::bind_back(std::plus<>{}, scalar));
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator-=(T scalar) noexcept
    {
        std::ranges::transform(this->m_data, this->m_data.begin(), std::bind_back(std::minus<>{}, scalar));
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator*=(T scalar) noexcept
    {
        std::ranges::transform(this->m_data, this->m_data.begin(), std::bind_back(std::multiplies<>{}, scalar));
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>& Mat<Rows, Cols, T, O>::operator/=(T scalar) noexcept
    {
        std::ranges::transform(this->m_data, this->m_data.begin(), std::bind_back(std::divides<>{}, scalar));
        return *this;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Vec<Rows, T> Mat<Rows, Cols, T, O>::operator*(const Vec<Cols, T>& vec) const noexcept
    {
        Vec<Rows, T> result{};
        for (std::size_t i = 0; i < Rows; ++i)
        {
            T sum{};
            for (std::size_t j = 0; j < Cols; ++j)
                sum += (*this)[i, j] * vec[j];
            result[i] = sum;
        }
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr void Mat<Rows, Cols, T, O>::transpose() noexcept requires (Rows == Cols)
    {
        Mat<Rows, Cols, T, O> tmp = *this;
        for (std::size_t r = 0; r < Rows; ++r)
            for (std::size_t c = 0; c < Cols; ++c)
                (*this)[r, c] = tmp[c, r];
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr void Mat<Rows, Cols, T, O>::invert() noexcept requires (Rows == Cols)
    {
        Mat<Rows, Cols, T, O> a = *this;
        Mat<Rows, Cols, T, O> b = Mat<Rows, Cols, T, O>::Identity();

        for (std::size_t i = 0; i < Rows; ++i)
        {
            T pivot = a[i, i];
            if (isZero(pivot) == true)
            {
                bool swapped = false;
                for (std::size_t j = i + 1; j < Rows; ++j)
                {
                    if (isZero(a[j, i]) == false)
                    {
                        for (std::size_t k = 0; k < Cols; ++k)
                        {
                            std::swap(a[i, k], a[j, k]);
                            std::swap(b[i, k], b[j, k]);
                        }
                        pivot = a[i, i];
                        swapped = true;
                        break;
                    }
                }
                if (!swapped)
                {
                    assert(false);
                    return;
                }
            }

            for (std::size_t k = 0; k < Cols; ++k)
            {
                a[i, k] /= pivot;
                b[i, k] /= pivot;
            }

            for (std::size_t j = 0; j < Rows; ++j)
            {
                if (j == i) continue;
                T factor = a[j, i];
                for (std::size_t k = 0; k < Cols; ++k)
                {
                    a[j, k] -= factor * a[i, k];
                    b[j, k] -= factor * b[i, k];
                }
            }
        }

        *this = b;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::transposed() const noexcept requires (Rows == Cols)
    {
        Mat<Rows, Cols, T, O> result{*this};
        result.transpose();
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O> Mat<Rows, Cols, T, O>::inverted() const noexcept requires (Rows == Cols)
    {
        Mat<Rows, Cols, T, O> result{*this};
        result.invert();
        return result;
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>::template VecView<Mat<Rows, Cols, T, O>::Axis::Row> Mat<Rows, Cols, T, O>::row(std::size_t row) const noexcept
    {
        assert(row < Rows);
        return VecView<Axis::Row>{ *this, row };
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>::template VecView<Mat<Rows, Cols, T, O>::Axis::Row> Mat<Rows, Cols, T, O>::row(std::size_t row) noexcept
    {
        assert(row < Rows);
        return VecView<Axis::Row>{ *this, row };
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>::template VecView<Mat<Rows, Cols, T, O>::Axis::Column> Mat<Rows, Cols, T, O>::column(std::size_t column) const noexcept
    {
        assert(column < Cols);
        return VecView<Axis::Column>{ *this, column };
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr Mat<Rows, Cols, T, O>::template VecView<Mat<Rows, Cols, T, O>::Axis::Column> Mat<Rows, Cols, T, O>::column(std::size_t column) noexcept
    {
        assert(column < Cols);
        return VecView<Axis::Column>{ *this, column };
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    template<class Func> requires (std::invocable<Func&, T&> && std::same_as<std::invoke_result_t<Func&, T&>, void>)
    constexpr void Mat<Rows, Cols, T, O>::row(std::size_t row, Func&& func) noexcept
    {
        for (T& data : this->row(row))
            std::invoke(func, data);
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    template<class Func> requires (std::invocable<Func&, T&> && std::same_as<std::invoke_result_t<Func&, T&>, void>)
    constexpr void Mat<Rows, Cols, T, O>::column(std::size_t column, Func&& func) noexcept
    {
        for (T& data : this->column(column))
            std::invoke(func, data);
    }

    template<std::size_t Rows, std::size_t Cols, Number T, MatrixOrdering O>
    constexpr std::size_t Mat<Rows, Cols, T, O>::index(std::size_t row, std::size_t col) noexcept
    {
        assert(row < Rows && col < Cols);
        if constexpr (O == MatrixOrdering::RowMajor)
            return row * Cols + col;
        else
            return col * Rows + row;
    }
}