#pragma once
#include "Math/matrix.hpp"
#include "Math/quaternion.hpp"

struct Transform
{
private:
    FMatrix4    globalMatrix;
    FQuaternion rotation;
    FVector3    position;
    FVector3    scale;

    Bool        hasChanged;

public:
    Transform(const FVector3    &transformPosition = { 0.0f, 0.0f, 0.0f }, 
              const FQuaternion &transformRotation = { 0.0f, 0.0f, 0.0f, 1.0f },
              const FVector3    &transformScale    = { 1.0f, 1.0f, 1.0f })
        : globalMatrix(1.0f)
        , rotation(transformRotation)
        , position(transformPosition)
        , scale(transformScale)
        , hasChanged(true)
    {}

    Void set_changed()
    {
        hasChanged = true;
    }

    Void set_global_position(const FVector3 &globalPosition)
    {
        const FMatrix4 invertedParentGlobalMatrix = Math::inverse(get_global_parent_matrix());

        position = FVector3(invertedParentGlobalMatrix * FVector4(globalPosition, 1.0f));
        hasChanged = true;
    }

    Void set_local_position(const FVector3 &localPosition)
    {
        position = localPosition;
        hasChanged = true;
    }



    Void set_global_euler_radians(const FVector3 &globalEulerAnglesRadians)
    {
        set_global_rotation(FQuaternion(globalEulerAnglesRadians));
    }

    Void set_local_euler_radians(const FVector3 &localEulerAnglesRadians)
    {
        set_local_rotation(FQuaternion(localEulerAnglesRadians));
    }

    Void set_global_euler_degrees(const FVector3 &globalEulerAnglesDegrees)
    {
        set_global_rotation(FQuaternion(Math::to_radians(globalEulerAnglesDegrees)));
    }

    Void set_local_euler_degrees(const FVector3 &localEulerAnglesDegrees)
    {
        set_local_rotation(FQuaternion(Math::to_radians(localEulerAnglesDegrees)));
    }


    Void set_global_rotation(const FQuaternion &globalRotation)
    {
        const FMatrix4 parentGlobalMatrix = get_global_parent_matrix();
        const FQuaternion parentRotation = Math::extract_rotation(parentGlobalMatrix);

        rotation = Math::inverse(parentRotation) * globalRotation;

        hasChanged = true;
    }

    Void set_local_rotation(const FQuaternion &localRotation)
    {
        rotation = localRotation;
        hasChanged = true;
    }


    Void set_global_scale(const FVector3 &globalScale)
    {
        const FMatrix4 parentGlobalMatrix = get_global_parent_matrix();
        const FVector3 parentScale = Math::extract_scale(parentGlobalMatrix);

        scale = globalScale / parentScale;

        hasChanged = true;
    }

    Void set_local_scale(const FVector3 &localScale)
    {
        scale = localScale;
        hasChanged = true;
    }




    Bool update_global_matrix(const FMatrix4 &parentGlobalMatrix, const Bool hasParentChanged = false)
    {
        if (hasChanged || hasParentChanged)
        {
            globalMatrix = orthonormal_multiply(parentGlobalMatrix);
            hasChanged = false;
            return true;
        }
        return false;
    }

    Bool update_global_matrix()
    {
        if (hasChanged)
        {
            globalMatrix = get_local_matrix();
            hasChanged = false;
            return true;
        }
        return false;
    }



    [[nodiscard]]
    FMatrix4 get_local_matrix() const
    {
        FMatrix4 localMatrix{ 1.0f };
        localMatrix =  Math::translate(localMatrix, position);
        localMatrix *= Math::to_matrix(rotation);
        localMatrix =  Math::scale(localMatrix, scale);

        return localMatrix;
    }

    [[nodiscard]]
    const FMatrix4 &get_global_matrix() const
    {
        return globalMatrix;
    }



    [[nodiscard]]
    FVector3 get_global_position() const
    {
        return { globalMatrix[3][0], globalMatrix[3][1], globalMatrix[3][2] };
    }

    [[nodiscard]]
    const FVector3 &get_local_position() const
    {
        return position;
    }



    [[nodiscard]]
    FVector3 get_global_euler_rotation_radians() const
    {
        return Math::to_euler_radians(get_global_rotation());
    }

    [[nodiscard]]
    FVector3 get_local_euler_rotation_radians() const
    {
        return Math::to_euler_radians(rotation);
    }

    [[nodiscard]]
    FVector3 get_global_euler_rotation_degrees() const
    {
        return Math::to_degrees(Math::to_euler_radians(get_global_rotation()));
    }

    [[nodiscard]]
    FVector3 get_local_euler_rotation_degrees() const
    {
        return Math::to_degrees(Math::to_euler_radians(rotation));
    }


    [[nodiscard]]
    FQuaternion get_global_rotation() const
    {
        return Math::extract_rotation(globalMatrix);
    }

    [[nodiscard]]
    const FQuaternion &get_local_rotation() const
    {
        return rotation;
    }



    [[nodiscard]]
    FVector3 get_global_scale() const
    {
        return Math::extract_scale(globalMatrix);
    }

    [[nodiscard]]
    const FVector3 &get_local_scale() const
    {
        return scale;
    }

private:
    [[nodiscard]]
    FMatrix4 get_global_parent_matrix() const
    {
        return globalMatrix * Math::inverse(get_local_matrix());
    }

    FMatrix4 orthonormal_multiply(const FMatrix4 &parent) const
    {
        const FVector3 parentScale = Math::extract_scale(parent);
        const FMatrix4 parentWithoutScale = Math::remove_scale(parent);

        const FMatrix4 localWithoutScale = Math::translate({ 1.0f }, position)
                                         * Math::to_matrix(rotation);

        FMatrix4 orthonormalGlobal = parentWithoutScale * localWithoutScale;
        orthonormalGlobal = Math::scale(orthonormalGlobal, parentScale * scale);

        return orthonormalGlobal;
    }
};
