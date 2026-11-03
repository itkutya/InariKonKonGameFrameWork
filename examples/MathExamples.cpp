#include <cassert>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <numbers>
#include <type_traits>

import Math;

namespace
{
    constexpr bool near(float lhs, float rhs, float tolerance = 0.0001f)
    {
        return std::abs(lhs - rhs) <= tolerance;
    }
}

int main()
{
    using namespace ikk;

    // Concepts, approximate zero checks, and named casts.
    static_assert(BasicNumber<int> && BasicNumber<float> && !BasicNumber<bool>);
    static_assert(Number<Degreef> && FloatingPointNumber<double>);
    static_assert(isZero(0) && !isZero(1) && isZero(0.001f, 0.01f));
    static_assert(std::same_as<decltype(U32(42)), std::uint32_t>);
    static_assert(std::same_as<decltype(I64(42)), std::int64_t>);
    static_assert(std::same_as<decltype(F32(42)), float>);
    static_assert(U8(258) == 2 && BOOL(1));

    // Clamped values use [0, 1] for floating point and the type range for integers.
    {
        constexpr Clamped<float> low{-1.0f}, middle{0.5f}, high{2.0f};
        static_assert(low.value() == 0.0f && middle.value() == 0.5f && high.value() == 1.0f);
        static_assert((middle + 0.75f).value() == 1.0f);
        static_assert((middle - 0.75f).value() == 0.0f);
        static_assert((middle * 0.5f).value() == 0.25f);
        static_assert(Clamped<int, -10, 10>{7}.value() == 7);
        Clamped<float> value{0.25f};
        value += 0.25f;
        value *= 2.0f;
        assert(value.value() == 1.0f);
    }

    // Vectors: dimensions, access, arithmetic, products, and normalization.
    {
        constexpr Vec2f a{3.0f, 4.0f};
        constexpr Vec2f b{1.0f, 2.0f};
        static_assert(Vec2f::Dimension == 2 && Vec3f::Z() == Vec3f{0.0f, 0.0f, 1.0f});
        static_assert((a + b) == Vec2f{4.0f, 6.0f});
        static_assert((a - b) == Vec2f{2.0f, 2.0f});
        static_assert((a * 2.0f) == Vec2f{6.0f, 8.0f});
        static_assert((a / 2.0f) == Vec2f{1.5f, 2.0f});
        static_assert(a.dot(b) == 11.0f && a.cross(b) == 2.0f);
        static_assert(a.length_squared() == 25.0f);
        static_assert(a[0] == 3.0f && a.at<1>() == 4.0f);
        constexpr Vec3f x{1.0f, 2.0f, 3.0f}, y{4.0f, 5.0f, 6.0f};
        static_assert(x.cross(y) == Vec3f{-3.0f, 6.0f, -3.0f});
        static_assert(x.dot(y) == 32.0f);
        static_assert((x * y) == Vec3f{4.0f, 10.0f, 18.0f});
        Vec2f normalized = a.normalized();
        assert(near(normalized.length(), 1.0f));
        normalized.normalize();
        assert(near(normalized.length(), 1.0f));
        assert(near(a.length(), 5.0f));
        assert(near(a.distance(b), std::sqrt(8.0f)));
        Vec2f arithmetic{1.0f, 2.0f};
        arithmetic += b;
        arithmetic *= 2.0f;
        assert((arithmetic == Vec2f{4.0f, 8.0f}));
    }

    // Matrices: construction, storage ordering views, arithmetic, transforms, and inversion.
    {
        constexpr Mat3x3f identity = Mat3x3f::Identity();
        constexpr Vec3f vector{1.0f, 2.0f, 3.0f};
        static_assert(identity[0, 0] == 1.0f && identity[0, 1] == 0.0f);
        static_assert(identity * vector == vector);
        constexpr Mat<2, 2, int> base{1, 2, 3, 4};
        static_assert((base + Mat<2, 2, int>{1, 1, 1, 1}) == Mat<2, 2, int>{2, 3, 4, 5});
        constexpr Mat<2, 2, int> product = base * Mat<2, 2, int>::Identity();
        static_assert(product == base);
        assert((base.transposed()[0, 1] == 3));
        Mat<2, 2, int> matrix{1, 2, 3, 4};
        assert(matrix.row(0)[0] == 1 && matrix.row(0)[1] == 2);
        assert(matrix.column(1)[0] == 2 && matrix.column(1)[1] == 4);
        Mat<2, 2, float> invertible{4.0f, 7.0f, 2.0f, 6.0f};
        const auto inverse = invertible.inverted();
        const auto check = invertible * inverse;
        assert(near(check[0, 0], 1.0f) && near(check[1, 1], 1.0f));
        matrix.row(0)[0] += 1;
        assert((matrix[0, 0] == 2));
    }

    // Angles and literals provide float and double degree/radian conversions.
    {
        constexpr auto halfTurn = 180.0_deg;
        constexpr auto pi = halfTurn.toRadian();
        static_assert(std::same_as<std::remove_cvref_t<decltype(halfTurn)>, Degreef>);
        static_assert(pi.value == std::numbers::pi_v<float>);
        static_assert((1.0_radd).toDegree().value == 180.0 / std::numbers::pi);
        static_assert((90_deg).toRadian().toDegree().value == 90.0f);
        static_assert((1.0_radf).value == 1.0f && (2.0_degd).value == 2.0);
    }

    // Quaternion constructors, composition, vector rotation, and matrix conversion.
    {
        constexpr Quaternionf identity = Quaternionf::Identity();
        static_assert(identity.x == 0.0f && identity.y == 0.0f && identity.z == 0.0f && identity.w == 1.0f);
        const Quaternionf quarterTurn{90.0_deg, Vec3f::Z()};
        const Vec3f rotated = quarterTurn * Vec3f::X();
        assert(near(rotated.x(), 0.0f) && near(rotated.y(), 1.0f));
        const auto roundTrip = Quaternionf{quarterTurn.toMat3x3()};
        assert(near(std::abs(roundTrip.w), std::abs(quarterTurn.w)));
        assert(near((quarterTurn * quarterTurn.conjugated()).w, 1.0f));
        assert(near(quarterTurn.normalized().length(), 1.0f));
        const EulerAnglesDegf euler{.yaw = 90.0_degf, .pitch = 0.0_degf, .roll = 0.0_degf};
        const Quaternionf fromEuler{euler};
        assert(near(fromEuler.length(), 1.0f));
    }

    // Rectangles support scalar/vector construction and axis-aligned or rotated containment.
    {
        constexpr Rectf rect{10.0f, 20.0f, 100.0f, 50.0f};
        static_assert(rect.getRight() == 110.0f && rect.getBottom() == 70.0f);
        static_assert(rect.contains(Vec2f{50.0f, 40.0f}));
        static_assert(!rect.contains(Vec2f{200.0f, 40.0f}));
        constexpr Rectf fromVectors{Vec2f{1.0f, 2.0f}, Vec2f{3.0f, 4.0f}};
        static_assert(fromVectors.contains(Vec2i{4, 6}));
        const Rectf centered{0.0f, 0.0f, 2.0f, 2.0f};
        const Quaternionf rotation{45.0_deg, Vec3f::Z()};
        assert(centered.contains(Vec2f{1.0f, 0.0f}, rotation, Vec2<Clamped<float, 0.0f, 1.0f>>{0.5f, 0.5f}));
    }

    // World directions are dimension-specific getter functions.
    static_assert(World2D::getRight() == Vec2f{1.0f, 0.0f});
    static_assert(World2D::getDown() == Vec2f{0.0f, -1.0f});
    static_assert(World3D::getRight() == Vec3f{1.0f, 0.0f, 0.0f});
    static_assert(World3D::getUp() == Vec3f{0.0f, 1.0f, 0.0f});
    static_assert(World3D::getForward() == Vec3f{0.0f, 0.0f, 1.0f});
    static_assert(World3D::getBackward() == -World3D::getForward());

    return 0;
}
