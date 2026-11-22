module;

#include <cassert>
#include <cmath>
#include <cstdint>

export module Camera;

import EulerAngles;
import Transform;
import Entity;
import Vector;
import Matrix;
import Angle;
import Rect;

export namespace ikk
{
    enum class Projection : std::uint8_t
    {
        Orthographic, Perspective
    };

    template<Projection Projection>
    class Camera final
    {
    public:
        Camera() noexcept = default;
        Camera(Vec3f position, EulerAnglesDegf angle, Degreef field_of_view = Degreef{ . value = 45.f }, float near = 0.1f, float far = 100.f) noexcept;

        Camera(const Camera&) noexcept = default;
        Camera(Camera&&) noexcept = default;

        Camera& operator=(const Camera&) noexcept = default;
        Camera& operator=(Camera&&) noexcept = default;

        ~Camera() noexcept = default;

        [[nodiscard]] const Transform3D& getTransform() const noexcept;
        [[nodiscard]] Transform3D& getTransform() noexcept;

        [[nodiscard]] float getNearPlane() const noexcept;
        [[nodiscard]] float getFarPlane() const noexcept;
        [[nodiscard]] Degreef getFieldOfView() const noexcept;

        void setNearPlane(float near) noexcept;
        void setFarPlane(float far) noexcept;
        void setFieldOfView(Degreef field_of_view) noexcept;

        [[nodiscard]] Mat4x4f getViewMatrix() const noexcept;

        [[nodiscard]] Mat4x4f getProjectionMatrix(float aspectRatio) const noexcept requires (Projection == Projection::Perspective);
        [[nodiscard]] Mat4x4f getProjectionMatrix(Rectf viewRect) const noexcept requires (Projection == Projection::Orthographic);
    private:
        Transform3D m_transform{};
        float m_near = 0.1f;
        float m_far = 100.f;
        Degreef m_field_of_view = Degreef{ .value = 45.f };
    };

    using PerspectiveCamera = Camera<Projection::Perspective>;
    using OrthographicCamera = Camera<Projection::Orthographic>;
}

namespace ikk
{
    template<Projection Projection>
    Camera<Projection>::Camera(Vec3f position, EulerAnglesDegf angle, Degreef field_of_view, float near, float far) noexcept
        : m_transform(position, angle), m_near(near), m_far(far), m_field_of_view(field_of_view)
    {
        assert(near > 0.f);
        assert(far > near);
    }

    template<Projection Projection>
    const Transform3D& Camera<Projection>::getTransform() const noexcept
    {
        return this->m_transform;
    }

    template<Projection Projection>
    Transform3D& Camera<Projection>::getTransform() noexcept
    {
        return this->m_transform;
    }

    template<Projection Projection>
    float Camera<Projection>::getNearPlane() const noexcept
    {
        return this->m_near;
    }

    template<Projection Projection>
    float Camera<Projection>::getFarPlane() const noexcept
    {
        return this->m_far;
    }

    template<Projection Projection>
    Degreef Camera<Projection>::getFieldOfView() const noexcept
    {
        return this->m_field_of_view;
    }

    template<Projection Projection>
    void Camera<Projection>::setNearPlane(float near) noexcept
    {
        this->m_near = near;
    }

    template<Projection Projection>
    void Camera<Projection>::setFarPlane(float far) noexcept
    {
        this->m_far = far;
    }

    template<Projection Projection>
    void Camera<Projection>::setFieldOfView(Degreef field_of_view) noexcept
    {
        this->m_field_of_view = field_of_view;
    }

    template<Projection Projection>
    Mat4x4f Camera<Projection>::getViewMatrix() const noexcept
    {
        const Vec3f up = this->m_transform.getUp();
        const Vec3f right = this->m_transform.getRight();
        const Vec3f direction = this->m_transform.getForward();
        const Vec3f position = this->m_transform.getWorldPosition();

        Mat4x4f result{1.f};

        result.at<0, 0>() = right.x();
        result.at<0, 1>() = right.y();
        result.at<0, 2>() = right.z();
        result.at<0, 3>() = -right.dot(position);

        result.at<1, 0>() = up.x();
        result.at<1, 1>() = up.y();
        result.at<1, 2>() = up.z();
        result.at<1, 3>() = -up.dot(position);

        result.at<2, 0>() = -direction.x();
        result.at<2, 1>() = -direction.y();
        result.at<2, 2>() = -direction.z();
        result.at<2, 3>() = direction.dot(position);

        return result;
    }

    template<Projection Projection>
    Mat4x4f Camera<Projection>::getProjectionMatrix(float aspectRatio) const noexcept requires (Projection == ikk::Projection::Perspective)
    {
        const float tanHalfFov = std::tanf(this->m_field_of_view.toRadian().value / 2.f);

        Mat4x4f result{0.f};

        result.at<0, 0>() = 1.f / (aspectRatio * tanHalfFov);

        result.at<1, 1>() = 1.f / tanHalfFov;

        result.at<2, 2>() = -(this->m_far + this->m_near) / (this->m_far - this->m_near);
        result.at<2, 3>() = -1.f;

        result.at<3, 2>() = -(2.f * this->m_far * this->m_near) / (this->m_far - this->m_near);

        return result;
    }

    template<Projection Projection>
    Mat4x4f Camera<Projection>::getProjectionMatrix(Rectf viewRect) const noexcept requires (Projection == ikk::Projection::Orthographic)
    {
        const float left = viewRect.getLeft();
        const float top = viewRect.getTop();
        const float right = viewRect.getRight();
        const float bottom = viewRect.getBottom();

        Mat4x4f result{1.f};

        result.at<0, 0>() = 2.f / viewRect.getWidth();
        result.at<1, 1>() = 2.f / viewRect.getHeight();
        result.at<2, 2>() = -2.f / (this->m_far - this->m_near);

        result.at<0, 3>() = -(right + left) / viewRect.getWidth();
        result.at<1, 3>() = -(bottom + top) / viewRect.getHeight();
        result.at<2, 3>() = -(this->m_far + this->m_near) / (this->m_far - this->m_near);

        return result;
    }
}