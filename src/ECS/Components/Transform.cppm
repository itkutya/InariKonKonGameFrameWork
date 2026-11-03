module;

#include <variant>

export module Transform;

import Hierarchy;

import WorldVectors;
import EulerAngles;
import Quaternion;
import Dimension;
import Number;
import Vector;
import Matrix;
import Angle;

namespace ikk
{
    template<Dimension D>
    struct TransformData;

    template<>
    struct TransformData<Dimension::_2D>
    {
        using VecType       = Vec2f;
        using MatType       = Mat3x3f;

        using PositionType  = Vec2f;
        using RotationType  = Quaternionf;
        using ScaleType     = Vec2f;

        using HomogeneousVecType = Vec3f;
    };

    template<>
    struct TransformData<Dimension::_3D>
    {
        using VecType       = Vec3f;
        using MatType       = Mat4x4f;

        using PositionType  = Vec3f;
        using RotationType  = Quaternionf;
        using ScaleType     = Vec3f;

        using HomogeneousVecType = Vec4f;
    };
}

export namespace ikk
{
    template<Dimension D>
    class [[nodiscard]] Transform final : public Hierarchy<Transform<D>>, public TransformData<D>
    {
    public:
        using typename TransformData<D>::VecType;
        using typename TransformData<D>::MatType;

        using typename TransformData<D>::PositionType;
        using typename TransformData<D>::RotationType;
        using typename TransformData<D>::ScaleType;

        using typename TransformData<D>::HomogeneousVecType;

        [[nodiscard]] constexpr Transform() noexcept = default;
        [[nodiscard]] constexpr Transform(PositionType position, EulerAnglesDegf rotation, ScaleType scale = ScaleType(1.f)) noexcept;
        [[nodiscard]] explicit constexpr Transform(PositionType position, EulerAnglesRadf rotation = {}, ScaleType scale = ScaleType(1.f)) noexcept;

        constexpr Transform(const Transform&) noexcept = default;
        constexpr Transform(Transform&&) noexcept = default;

        constexpr Transform& operator=(const Transform&) noexcept = default;
        constexpr Transform& operator=(Transform&&) noexcept = default;

        constexpr ~Transform() noexcept = default;

        [[nodiscard]] constexpr const MatType& getLocalMatrix() const noexcept;
        [[nodiscard]] constexpr const MatType& getWorldMatrix() const noexcept;

        [[nodiscard]] constexpr const PositionType& getLocalPosition() const noexcept;
        [[nodiscard]] constexpr const RotationType& getLocalRotation() const noexcept;
        [[nodiscard]] constexpr const ScaleType& getLocalScale() const noexcept;

        [[nodiscard]] constexpr PositionType getWorldPosition() const noexcept;
        [[nodiscard]] constexpr RotationType getWorldRotation() const noexcept;
        [[nodiscard]] constexpr ScaleType getWorldScale() const noexcept;

        [[nodiscard]] constexpr VecType getRight() const noexcept;
        [[nodiscard]] constexpr VecType getUp() const noexcept;
        [[nodiscard]] constexpr VecType getForward() const noexcept;

        constexpr void translate(PositionType position) noexcept;
        constexpr void translateLocal(PositionType position) noexcept;
        constexpr void translateGlobal(PositionType position) noexcept;
        constexpr void setWorldPosition(PositionType position) noexcept;
        constexpr void setLocalPosition(PositionType position) noexcept;

        constexpr void rotateLocal(VecType axes, Degree<float> degree) noexcept requires(D == Dimension::_3D);
        constexpr void rotateLocal(VecType axes, Radian<float> radian) noexcept requires(D == Dimension::_3D);
        constexpr void rotateGlobal(VecType axes, Degree<float> degree) noexcept requires(D == Dimension::_3D);
        constexpr void rotateGlobal(VecType axes, Radian<float> radian) noexcept requires(D == Dimension::_3D);

        constexpr void rotateLocal(Degree<float> degree) noexcept requires(D == Dimension::_2D);
        constexpr void rotateLocal(Radian<float> radian) noexcept requires(D == Dimension::_2D);
        constexpr void rotateGlobal(Degree<float> degree) noexcept requires(D == Dimension::_2D);
        constexpr void rotateGlobal(Radian<float> radian) noexcept requires(D == Dimension::_2D);

        constexpr void setLocalScale(ScaleType scale) noexcept;
        constexpr void setWorldScale(ScaleType scale) noexcept;
    private:
        PositionType m_localPosition = PositionType(0.f);
        RotationType m_localRotation = RotationType();
        ScaleType m_localScale = ScaleType(1.f);

        mutable bool m_dirty = true;
        mutable MatType m_localMatrix = MatType::Identity();
        mutable MatType m_worldMatrix = MatType::Identity();

        constexpr MatType getTranslationMatrix() const noexcept;
        constexpr MatType getRotationMatrix() const noexcept;
        constexpr MatType getScaleMatrix() const noexcept;

        constexpr void recalculateMatrices() const noexcept;
        void markDirty() noexcept;
    };

    using Transform2D = Transform<Dimension::_2D>;
    using Transform3D = Transform<Dimension::_3D>;
    using AnyTransform = std::variant<Transform2D, Transform3D>;
}

namespace ikk
{
    template<Dimension D>
    constexpr Transform<D>::Transform(PositionType position, EulerAnglesDegf rotation, ScaleType scale) noexcept
        : m_localPosition(position), m_localRotation(rotation), m_localScale(scale)
    {
    }

    template<Dimension D>
    constexpr Transform<D>::Transform(PositionType position, EulerAnglesRadf rotation, ScaleType scale) noexcept
        : m_localPosition(position), m_localRotation(rotation), m_localScale(scale)
    {
    }

    template<Dimension D>
    constexpr const Transform<D>::MatType& Transform<D>::getLocalMatrix() const noexcept
    {
        if (this->m_dirty == true)
            this->recalculateMatrices();
        return this->m_localMatrix;
    }

    template<Dimension D>
    constexpr const Transform<D>::MatType& Transform<D>::getWorldMatrix() const noexcept
    {
        if (this->m_dirty == true)
            this->recalculateMatrices();
        return this->m_worldMatrix;
    }

    template<Dimension D>
    constexpr const Transform<D>::PositionType& Transform<D>::getLocalPosition() const noexcept
    {
        return this->m_localPosition;
    }

    template<Dimension D>
    constexpr const Transform<D>::RotationType& Transform<D>::getLocalRotation() const noexcept
    {
        return this->m_localRotation;
    }

    template<Dimension D>
    constexpr const Transform<D>::ScaleType& Transform<D>::getLocalScale() const noexcept
    {
        return this->m_localScale;
    }

    template<Dimension D>
    constexpr Transform<D>::PositionType Transform<D>::getWorldPosition() const noexcept
    {
        if (this->getParent() != nullptr)
            return this->getParent()->getWorldPosition() + (this->getParent()->getWorldRotation() * (this->getParent()->getWorldScale() * this->m_localPosition));
        return this->m_localPosition;
    }

    template<Dimension D>
    constexpr Transform<D>::RotationType Transform<D>::getWorldRotation() const noexcept
    {
        if (this->getParent() != nullptr)
            return this->getParent()->getWorldRotation() * this->m_localRotation;
        return this->m_localRotation;
    }

    template<Dimension D>
    constexpr Transform<D>::ScaleType Transform<D>::getWorldScale() const noexcept
    {
        if (this->getParent() != nullptr)
            return this->getParent()->getWorldScale() * this->m_localScale;
        return this->m_localScale;
    }

    template<Dimension D>
    constexpr Transform<D>::VecType Transform<D>::getRight() const noexcept
    {
        return VecType{this->getWorldRotation() * World<D>::getRight()};
    }

    template<Dimension D>
    constexpr Transform<D>::VecType Transform<D>::getUp() const noexcept
    {
        return VecType{this->getWorldRotation() * World<D>::getUp()};
    }

    template<Dimension D>
    constexpr Transform<D>::VecType Transform<D>::getForward() const noexcept
    {
        return VecType{this->getWorldRotation() * World<D>::getForward()};
    }

    template<Dimension D>
    constexpr void Transform<D>::translate(PositionType position) noexcept
    {
        this->m_localPosition += position;
        this->markDirty();
    }

    template<Dimension D>
    constexpr void Transform<D>::translateLocal(PositionType position) noexcept
    {
        this->m_localPosition += (this->m_localRotation * position);
        this->markDirty();
    }

    template<Dimension D>
    constexpr void Transform<D>::translateGlobal(PositionType position) noexcept
    {
        if (this->getParent() != nullptr)
        {
            MatType inv = this->getParent()->getWorldMatrix().getInverse();
            HomogeneousVecType transformed{position};

            transformed = inv * transformed;
            this->m_localPosition += PositionType{transformed};
            this->markDirty();
        }
        else
            this->translateLocal(position);
    }

    template<Dimension D>
    constexpr void Transform<D>::setWorldPosition(PositionType position) noexcept
    {
        if (this->getParent() != nullptr)
        {
            MatType inv = this->getParent()->getWorldMatrix().getInverse();
            HomogeneousVecType transformed{position};
            transformed[HomogeneousVecType::Dimension - 1] = 1.f;

            transformed = inv * transformed;
            this->m_localPosition = PositionType{transformed};
            this->markDirty();
        }
        else
            this->setLocalPosition(position);
    }

    template<Dimension D>
    constexpr void Transform<D>::setLocalPosition(PositionType position) noexcept
    {
        this->m_localPosition = position;
        this->markDirty();
    }

    template<Dimension D>
    constexpr void Transform<D>::rotateLocal(VecType axes, Degree<float> degree) noexcept requires(D == Dimension::_3D)
    {
        this->rotateLocal(axes, degree.toRadian());
    }

    template<Dimension D>
    constexpr void Transform<D>::rotateLocal(VecType axes, Radian<float> radian) noexcept requires(D == Dimension::_3D)
    {
        if (isZero(axes.length()))
            return;

        axes.normalize();
        this->m_localRotation *= Quaternionf{radian, axes};
        this->m_localRotation.normalize();
        this->markDirty();
    }

    template<Dimension D>
    constexpr void Transform<D>::rotateGlobal(VecType axes, Degree<float> degree) noexcept requires(D == Dimension::_3D)
    {
        this->rotateGlobal(axes, degree.toRadian());
    }

    template<Dimension D>
    constexpr void Transform<D>::rotateGlobal(VecType axes, Radian<float> radian) noexcept requires(D == Dimension::_3D)
    {
        if (isZero(axes.length()))
            return;

        axes.normalize();
        this->m_localRotation = Quaternionf{radian, axes} * this->m_localRotation;
        this->m_localRotation.normalize();
        this->markDirty();
    }

    template<Dimension D>
    constexpr void Transform<D>::rotateLocal(Degree<float> degree) noexcept requires(D == Dimension::_2D)
    {
        this->rotateLocal(degree.toRadian());
    }

    template<Dimension D>
    constexpr void Transform<D>::rotateLocal(Radian<float> radian) noexcept requires(D == Dimension::_2D)
    {
        this->m_localRotation *= Quaternionf{radian, World<D>::getForward()};
        this->m_localRotation.normalize();
        this->markDirty();
    }

    template<Dimension D>
    constexpr void Transform<D>::rotateGlobal(Degree<float> degree) noexcept requires(D == Dimension::_2D)
    {
        this->rotateGlobal(degree.toRadian());
    }

    template<Dimension D>
    constexpr void Transform<D>::rotateGlobal(Radian<float> radian) noexcept requires(D == Dimension::_2D)
    {
        this->m_localRotation = Quaternionf{radian, World<D>::getForward()} * this->m_localRotation;
        this->m_localRotation.normalize();
        this->markDirty();
    }

    template<Dimension D>
    constexpr void Transform<D>::setLocalScale(ScaleType scale) noexcept
    {
        this->m_localScale = scale;
        this->markDirty();
    }

    template<Dimension D>
    constexpr void Transform<D>::setWorldScale(ScaleType scale) noexcept
    {
        if (this->getParent() != nullptr && isZero(this->getParent()->getWorldScale().length()) == false)
        {
            this->m_localScale = scale / this->getParent()->getWorldScale();
            this->markDirty();
        }
        else
            this->setLocalScale(scale);
    }

    template<>
    constexpr TransformData<Dimension::_2D>::MatType Transform<Dimension::_2D>::getTranslationMatrix() const noexcept
    {
        MatType translation = MatType::Identity();
        translation.column(2) = Vec3f{ this->m_localPosition.x(), this->m_localPosition.y(), 1.f };
        return translation;
    }

    template<>
    constexpr TransformData<Dimension::_2D>::MatType Transform<Dimension::_2D>::getRotationMatrix() const noexcept
    {
        return this->m_localRotation.toMat3x3();
    }

    template<>
    constexpr TransformData<Dimension::_2D>::MatType Transform<Dimension::_2D>::getScaleMatrix() const noexcept
    {
        MatType scale = MatType::Identity();
        scale.at<0, 0>() = this->m_localScale.x();
        scale.at<1, 1>() = this->m_localScale.y();
        return scale;
    }

    template<>
    constexpr TransformData<Dimension::_3D>::MatType Transform<Dimension::_3D>::getTranslationMatrix() const noexcept
    {
        MatType translation = MatType::Identity();
        translation.column(3) = Vec4f{ this->m_localPosition.x(), this->m_localPosition.y(), this->m_localPosition.z(), 1.f };
        return translation;
    }

    template<>
    constexpr TransformData<Dimension::_3D>::MatType Transform<Dimension::_3D>::getRotationMatrix() const noexcept
    {
        return this->m_localRotation.toMat4x4();
    }

    template<>
    constexpr TransformData<Dimension::_3D>::MatType Transform<Dimension::_3D>::getScaleMatrix() const noexcept
    {
        MatType scale = MatType::Identity();
        scale.at<0, 0>() = this->m_localScale.x();
        scale.at<1, 1>() = this->m_localScale.y();
        scale.at<2, 2>() = this->m_localScale.z();
        return scale;
    }

    template<Dimension D>
    constexpr void Transform<D>::recalculateMatrices() const noexcept
    {
        MatType translation = this->getTranslationMatrix();
        MatType rotation    = this->getRotationMatrix();
        MatType scale       = this->getScaleMatrix();

        this->m_localMatrix = translation * rotation * scale;

        if (this->getParent() != nullptr)
            this->m_worldMatrix = this->getParent()->getWorldMatrix() * this->m_localMatrix;
        else
            this->m_worldMatrix = this->m_localMatrix;

        this->m_dirty = false;
    }

    template<Dimension D>
    void Transform<D>::markDirty() noexcept
    {
        this->m_dirty = true;
        for (Transform<D>* child : this->getChildren())
            child->markDirty();
    }
}