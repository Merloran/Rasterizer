#pragma once
class Camera
{
public:
	Void initialize(const FVector3& position, Float32 fovYDegrees, Float32 aspectRatio);

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

	Void set_position(const FVector3& position);
	Void set_view_bounds(const FVector2& viewBounds);
	Void set_fov(Float32 fov);

	Void update_camera_vectors();

private:
	FVector3 forward, up, right;
	FVector3 position;
	FVector2 lastPosition, viewBounds;
	Float32  yaw, pitch, fovY, aspectRatio;
};

