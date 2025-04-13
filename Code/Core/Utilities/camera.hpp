#pragma once
#include "Math/matrix.hpp"


class Camera
{
public:
	Void initialize(const FVector3& cameraPosition, Float32 cameraFovYDegrees, Float32 cameraAspectRatio);

	[[nodiscard]]
	FMatrix4 get_view() const;
	[[nodiscard]]
	FMatrix4 get_projection() const;

	[[nodiscard]]
	const FVector3& get_forward() const;
	[[nodiscard]]
	const FVector3& get_right() const;
	[[nodiscard]]
	const FVector3& get_up() const;
	[[nodiscard]]
	const FVector3& get_position() const;
	[[nodiscard]]
	const FVector2& get_view_bounds() const;
	[[nodiscard]]
	Float32 get_fov() const;

	Void set_position(const FVector3& cameraPosition);
	Void set_view_bounds(const FVector2& cameraViewBounds);
	Void set_fov_y(Float32 cameraFovY);

	Void update_camera_vectors();

private:
	FVector3 forward, up, right;
	FVector3 position;
	FVector2 lastPosition, viewBounds;
	Float32  yaw, pitch, fovY, aspectRatio;
};

