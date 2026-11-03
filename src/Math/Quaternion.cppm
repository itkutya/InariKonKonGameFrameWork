module;

#include <cmath>

export module Quaternion;

import EulerAngles;
import Number;
import Matrix;
import Vector;
import Angle;

export namespace ikk
{
    template<Number T>
    struct [[nodiscard]] Quaternion final
    {
        [[nodiscard]] constexpr Quaternion() noexcept = default;
        [[nodiscard]] constexpr Quaternion(T _x, T _y, T _z, T _w) noexcept;
        [[nodiscard]] explicit constexpr Quaternion(Vec3<T> vec, T _w) noexcept;
        [[nodiscard]] explicit constexpr Quaternion(EulerAnglesDeg<T> eulerDegrees) noexcept;
        [[nodiscard]] explicit constexpr Quaternion(EulerAnglesRad<T> eulerRadians) noexcept;
        [[nodiscard]] constexpr Quaternion(Degree<T> degree, Vec3<T> axis) noexcept;
        [[nodiscard]] constexpr Quaternion(Radian<T> radian, Vec3<T> axis) noexcept;
        [[nodiscard]] constexpr Quaternion(Degree<T> degree, Vec2<T> axis) noexcept;
        [[nodiscard]] constexpr Quaternion(Radian<T> radian, Vec2<T> axis) noexcept;
        [[nodiscard]] explicit constexpr Quaternion(const Mat3x3<T>& matrix) noexcept;
        [[nodiscard]] explicit constexpr Quaternion(const Mat4x4<T>& matrix) noexcept;

        constexpr Quaternion(const Quaternion& other) noexcept = default;
        constexpr Quaternion(Quaternion&& other) noexcept = default;

        constexpr Quaternion& operator=(const Quaternion& other) noexcept = default;
        constexpr Quaternion& operator=(Quaternion&& other) noexcept = default;

        constexpr ~Quaternion() noexcept = default;

        [[nodiscard]] static constexpr Quaternion Identity() noexcept;

        [[nodiscard]] constexpr Quaternion operator+(const Quaternion& other) const noexcept;
        [[nodiscard]] constexpr Quaternion operator-(const Quaternion& other) const noexcept;
        [[nodiscard]] constexpr Quaternion operator*(const Quaternion& other) const noexcept;

        [[nodiscard]] constexpr Vec2<T> operator*(Vec2<T> vec) const noexcept;
        [[nodiscard]] constexpr Vec3<T> operator*(Vec3<T> vec) const noexcept;

        [[nodiscard]] constexpr Quaternion operator*(T scale) const noexcept;
        [[nodiscard]] constexpr Quaternion operator/(T scale) const noexcept;

        [[nodiscard]] constexpr Quaternion operator-() const noexcept;

        constexpr Quaternion& operator+=(const Quaternion& other) noexcept;
        constexpr Quaternion& operator-=(const Quaternion& other) noexcept;
        constexpr Quaternion& operator*=(const Quaternion& other) noexcept;

        constexpr Quaternion& operator*=(T scale) noexcept;
        constexpr Quaternion& operator/=(T scale) noexcept;

        [[nodiscard]] constexpr bool operator==(const Quaternion& other) const noexcept;
        [[nodiscard]] constexpr bool operator!=(const Quaternion& other) const noexcept;

        constexpr void normalize() noexcept;
        constexpr void conjugate() noexcept;
        constexpr void invert() noexcept;

        [[nodiscard]] constexpr Quaternion normalized() const noexcept;
        [[nodiscard]] constexpr Quaternion conjugated() const noexcept;
        [[nodiscard]] constexpr Quaternion inverted() const noexcept;

        [[nodiscard]] constexpr T length_squared() const noexcept;
        [[nodiscard]] constexpr T length() const noexcept;

        [[nodiscard]] constexpr Mat3x3<T> toMat3x3() const noexcept;
        [[nodiscard]] constexpr Mat4x4<T> toMat4x4() const noexcept;

        T x{T{0}}, y{T{0}}, z{T{0}}, w{T{1}};
    };

    using Quaternionf = Quaternion<float>;
    using Quaterniond = Quaternion<double>;
}

namespace ikk
{
    template<Number T>
    constexpr Quaternion<T>::Quaternion(T _x, T _y, T _z, T _w) noexcept
        : x(_x), y(_y), z(_z), w(_w)
    {
    }

    template<Number T>
    constexpr Quaternion<T>::Quaternion(Vec3<T> vec, T _w) noexcept
        : Quaternion<T>(vec.x(), vec.y(), vec.z(), _w)
    {
    }

    template<Number T>
    constexpr Quaternion<T>::Quaternion(EulerAnglesDeg<T> eulerDegrees) noexcept
        : Quaternion<T>(EulerAnglesRad<T>{ eulerDegrees.yaw.toRadian(), eulerDegrees.pitch.toRadian(), eulerDegrees.roll.toRadian() })
    {
    }

    template<Number T>
    constexpr Quaternion<T>::Quaternion(EulerAnglesRad<T> eulerRadians) noexcept
    {
        const T halfYaw   = eulerRadians.yaw.value   * T{0.5f};
        const T halfPitch = eulerRadians.pitch.value * T{0.5f};
        const T halfRoll  = eulerRadians.roll.value  * T{0.5f};

        const T sinPitch = std::sin(halfPitch);
        const T cosPitch = std::cos(halfPitch);
        const T sinYaw   = std::sin(halfYaw);
        const T cosYaw   = std::cos(halfYaw);
        const T sinRoll  = std::sin(halfRoll);
        const T cosRoll  = std::cos(halfRoll);

        this->x = cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll;
        this->y = sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll;
        this->z = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll;
        this->w = cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;

        this->normalize();
    }

    template<Number T>
    constexpr Quaternion<T>::Quaternion(Degree<T> degree, Vec3<T> axis) noexcept
        : Quaternion<T>{degree.toRadian(), axis}
    {
    }

    template<Number T>
    constexpr Quaternion<T>::Quaternion(Radian<T> radian, Vec3<T> axis) noexcept
    {
        axis.normalize();
        const T halfAngle = radian.value / T{2.f};
        const T s = std::sin(halfAngle);
        
        this->x = axis.x() * s;
        this->y = axis.y() * s;
        this->z = axis.z() * s;
        this->w = std::cos(halfAngle);
    }

    template<Number T>
    constexpr Quaternion<T>::Quaternion(Degree<T> degree, Vec2<T> axis) noexcept
        : Quaternion<T>{degree.toRadian(), axis}
    {
    }

    template<Number T>
    constexpr Quaternion<T>::Quaternion(Radian<T> radian, Vec2<T> axis) noexcept
    {
        axis.normalize();
        const T halfAngle = radian / T{2.f};
        const T s = std::sin(halfAngle);
        
        this->x = axis.x() * s;
        this->y = axis.y() * s;
        this->z = T{0.f};
        this->w = std::cos(halfAngle);
    }

    template<Number T>
    constexpr Quaternion<T>::Quaternion(const Mat3x3<T>& matrix) noexcept
    {
        const T trace = matrix[0, 0] + matrix[1, 1] + matrix[2, 2];

        if (trace > T{0.0f}) 
        {
            const T s = std::sqrt(trace + T{1.0f}) * T{2.f};
            this->w = T{0.25f} * s;
            this->x = (matrix[2, 1] - matrix[1,2]) / s;
            this->y = (matrix[0, 2] - matrix[2,0]) / s;
            this->z = (matrix[1, 0] - matrix[0,1]) / s;
        }
        else if ((matrix[0, 0] > matrix[1, 1]) && (matrix[0, 0] > matrix[2, 2]))
        {
            const T s = std::sqrt(T{1.0f} + matrix[0, 0] - matrix[1, 1] - matrix[2, 2]) * T{2.f};
            this->w = (matrix[2, 1] - matrix[1, 2]) / s;
            this->x = T{0.25f} * s;
            this->y = (matrix[0, 1] + matrix[1, 0]) / s;
            this->z = (matrix[0, 2] + matrix[2, 0]) / s;
        } 
        else if (matrix[1, 1] > matrix[2, 2])
        {
            const T s = std::sqrt(T{1.0f} + matrix[1, 1] - matrix[0, 0] - matrix[2, 2]) * T{2.f};
            this->w = (matrix[0, 2] - matrix[2, 0]) / s;
            this->x = (matrix[0, 1] + matrix[1, 0]) / s;
            this->y = T{0.25f} * s;
            this->z = (matrix[1, 2] + matrix[2, 1]) / s;
        } 
        else 
        {
            const T s = std::sqrt(T{1.0f} + matrix[2, 2] - matrix[0, 0] - matrix[1, 1]) * T{2.f};
            this->w = (matrix[1, 0] - matrix[0, 1]) / s;
            this->x = (matrix[0, 2] + matrix[2, 0]) / s;
            this->y = (matrix[1, 2] + matrix[2, 1]) / s;
            this->z = T{0.25f} * s;
        }

        this->normalize();
    }

    template<Number T>
    constexpr Quaternion<T>::Quaternion(const Mat4x4<T>& matrix) noexcept
        : Quaternion<T>([](const Mat4x4<T>& mat) noexcept
        {
            Mat3x3<T> temp{};
            temp.column(0)[0] = mat.column(0)[0];
            temp.column(0)[1] = mat.column(0)[1];
            temp.column(0)[2] = mat.column(0)[2];

            temp.column(1)[0] = mat.column(1)[0];
            temp.column(1)[1] = mat.column(1)[1];
            temp.column(1)[2] = mat.column(1)[2];

            temp.column(2)[0] = mat.column(2)[0];
            temp.column(2)[1] = mat.column(2)[1];
            temp.column(2)[2] = mat.column(2)[2];

            return Quaternion<T>{ temp };
        }(matrix))
    {
    }

    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::Identity() noexcept
    {
        return Quaternion<T>{ T{0}, T{0}, T{0}, T{1} };
    }

    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::operator+(const Quaternion<T>& other) const noexcept
    {
        Quaternion<T> out{*this};
        out += other;
        return out;
    }

    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::operator-(const Quaternion<T>& other) const noexcept
    {
        Quaternion<T> out{*this};
        out -= other;
        return out;
    }
    
    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::operator*(const Quaternion<T>& other) const noexcept
    {
        Quaternion<T> out{*this};
        out *= other;
        return out;
    }

    template<Number T>
    constexpr Vec2<T> Quaternion<T>::operator*(Vec2<T> vec) const noexcept
    {
        const Quaternion<T> qVec{ vec.x(), vec.y(), T{0}, T{0} };
        const Quaternion<T> rotated = (*this) * qVec * this->inverted();
        return Vec2<T>{ rotated.x, rotated.y };
    }

    template<Number T>
    constexpr Vec3<T> Quaternion<T>::operator*(Vec3<T> vec) const noexcept
    {
        const Quaternion<T> qVec{ vec, T{0} };
        const Quaternion<T> rotated = (*this) * qVec * this->inverted();
        return Vec3<T>{ rotated.x, rotated.y, rotated.z };
    }

    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::operator*(T scale) const noexcept
    {
        Quaternion<T> out{*this};
        out *= scale;
        return out;
    }

    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::operator/(T scale) const noexcept
    {
        Quaternion<T> out{*this};
        out /= scale;
        return out;
    }

    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::operator-() const noexcept
    {
        return Quaternion<T>{ -this->x, -this->y, -this->z, -this->w };
    }

    template<Number T>
    constexpr Quaternion<T>& Quaternion<T>::operator+=(const Quaternion<T>& other) noexcept
    {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        this->w += other.w;
        return *this;
    }

    template<Number T>
    constexpr Quaternion<T>& Quaternion<T>::operator-=(const Quaternion<T>& other) noexcept
    {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->w -= other.w;
        return *this;
    }

    template<Number T>
    constexpr Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<T>& other) noexcept
    {
        const T oldX = this->x;
        const T oldY = this->y;
        const T oldZ = this->z;
        const T oldW = this->w;

        this->x = oldW * other.x + oldX * other.w + oldY * other.z - oldZ * other.y;
        this->y = oldW * other.y - oldX * other.z + oldY * other.w + oldZ * other.x;
        this->z = oldW * other.z + oldX * other.y - oldY * other.x + oldZ * other.w;
        this->w = oldW * other.w - oldX * other.x - oldY * other.y - oldZ * other.z;
        return *this;
    }

    template<Number T>
    constexpr Quaternion<T>& Quaternion<T>::operator*=(T scale) noexcept
    {
        this->x *= scale;
        this->y *= scale;
        this->z *= scale;
        this->w *= scale;
        return *this;
    }

    template<Number T>
    constexpr Quaternion<T>& Quaternion<T>::operator/=(T scale) noexcept
    {
        this->x /= scale;
        this->y /= scale;
        this->z /= scale;
        this->w /= scale;
        return *this;
    }

    template<Number T>
    constexpr bool Quaternion<T>::operator==(const Quaternion& other) const noexcept
    {
        return (isZero(this->x - other.x) == true &&
                isZero(this->y - other.y) == true &&
                isZero(this->z - other.z) == true &&
                isZero(this->w - other.w) == true);
    }

    template<Number T>
    constexpr bool Quaternion<T>::operator!=(const Quaternion& other) const noexcept
    {
        return !(*this == other);
    }

    template<Number T>
    constexpr void Quaternion<T>::normalize() noexcept
    {
        const T length = this->length();
        if (isZero(length) == false)
            (*this) /= length;
    }

    template<Number T>
    constexpr void Quaternion<T>::conjugate() noexcept
    {
        this->x = -this->x;
        this->y = -this->y;
        this->z = -this->z;
    }

    template<Number T>
    constexpr void Quaternion<T>::invert() noexcept
    {
        const T length = this->length_squared();
        if (isZero(length) == false)
        {
            this->x = -this->x / length;
            this->y = -this->y / length;
            this->z = -this->z / length;
            this->w = w / length;
        }
    }

    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::normalized() const noexcept
    {
        Quaternion<T> out{*this};
        out.normalize();
        return out;
    }

    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::conjugated() const noexcept
    {
        Quaternion<T> out{*this};
        out.conjugate();
        return out;
    }

    template<Number T>
    constexpr Quaternion<T> Quaternion<T>::inverted() const noexcept
    {
        Quaternion<T> out{*this};
        out.invert();
        return out;
    }

    template<Number T>
    constexpr T Quaternion<T>::length_squared() const noexcept
    {
        return Vec4<T>{ this->x, this->y, this->z, this->w }.length_squared();
    }

    template<Number T>
    constexpr T Quaternion<T>::length() const noexcept
    {
        return Vec4<T>{ this->x, this->y, this->z, this->w }.length();
    }

    template<Number T>
    constexpr Mat3x3<T> Quaternion<T>::toMat3x3() const noexcept
    {
        const T xx = this->x * this->x, yy = this->y * this->y, zz = this->z * this->z;
        const T xy = this->x * this->y, xz = this->x * this->z, yz = this->y * this->z;
        const T wx = this->w * this->x, wy = this->w * this->y, wz = this->w * this->z;

        Mat3x3<T> m = Mat3x3<T>::Identity();

        m[0, 0] = T{1.f} - T{2.f} * (yy + zz);
        m[0, 1] = T{2.f} * (xy - wz);
        m[0, 2] = T{2.f} * (xz + wy);

        m[1, 0] = T{2.f} * (xy + wz);
        m[1, 1] = T{1.f} - T{2.f} * (xx + zz);
        m[1, 2] = T{2.f} * (yz - wx);

        m[2, 0] = T{2.f} * (xz - wy);
        m[2, 1] = T{2.f} * (yz + wx);
        m[2, 2] = T{1.f} - T{2.f} * (xx + yy);

        return m;
    }

    template<Number T>
    constexpr Mat4x4<T> Quaternion<T>::toMat4x4() const noexcept
    {
        const T xx = this->x * this->x, yy = this->y * this->y, zz = this->z * this->z;
        const T xy = this->x * this->y, xz = this->x * this->z, yz = this->y * this->z;
        const T wx = this->w * this->x, wy = this->w * this->y, wz = this->w * this->z;

        Mat4x4<T> m = Mat4x4<T>::Identity();

        m[0, 0] = T{1.f} - T{2.f} * (yy + zz);
        m[0, 1] = T{2.f} * (xy - wz);
        m[0, 2] = T{2.f} * (xz + wy);

        m[1, 0] = T{2.f} * (xy + wz);
        m[1, 1] = T{1.f} - T{2.f} * (xx + zz);
        m[1, 2] = T{2.f} * (yz - wx);

        m[2, 0] = T{2.f} * (xz - wy);
        m[2, 1] = T{2.f} * (yz + wx);
        m[2, 2] = T{1.f} - T{2.f} * (xx + yy);

        return m;
    }
}