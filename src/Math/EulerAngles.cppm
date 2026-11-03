export module EulerAngles;

import Number;
import Angle;

export namespace ikk
{
    template<AngleTypes T>
    struct [[nodiscard]] EulerAngles
    {
        T yaw{};
        T pitch{};
        T roll{};
    };

    template<FloatingPointNumber T> using EulerAnglesDeg = EulerAngles<Degree<T>>;
    template<FloatingPointNumber T> using EulerAnglesRad = EulerAngles<Radian<T>>;

    using EulerAnglesDegf = EulerAnglesDeg<float>;
    using EulerAnglesDegd = EulerAnglesDeg<double>;
    using EulerAnglesRadf = EulerAnglesRad<float>;
    using EulerAnglesRadd = EulerAnglesRad<double>;
}