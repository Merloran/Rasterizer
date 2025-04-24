#include "camera.hpp"

Void Camera::initialize(const FVector3 &cameraPosition, const Float32 cameraFovYDegrees, const Float32 cameraAspectRatio)
{
	position = cameraPosition;
	fovY = cameraFovYDegrees;
	aspectRatio = cameraAspectRatio;
	forward = { 0.0f,  0.0f, 1.0f };
	up = { 0.0f,  1.0f,  0.0f };
	right = { 1.0f,  0.0f,  0.0f };
	viewBounds = { 0.1f,  5000.0f };
	yaw   = -90.0f;
	pitch = 0.0f;

	update_camera_vectors();
}


FMatrix4 Camera::get_view() const
{
	return Math::look_at(position, position + forward, up);
}

FMatrix4 Camera::get_projection() const
{
	return Math::perspective(Math::to_radians(fovY), aspectRatio, viewBounds.x, viewBounds.y);
}

const FVector3& Camera::get_forward() const
{
	return forward;
}

const FVector3& Camera::get_right() const
{
	return right;
}

const FVector3& Camera::get_up() const
{
	return up;
}

const FVector3& Camera::get_position() const
{
	return position;
}

const FVector2& Camera::get_view_bounds() const
{
	return viewBounds;
}

Float32 Camera::get_fov() const
{
	return fovY;
}

Void Camera::set_position(const FVector3 &cameraPosition)
{
	position = cameraPosition;
}

Void Camera::set_view_bounds(const FVector2 &cameraViewBounds)
{
	viewBounds = cameraViewBounds;
}

Void Camera::set_fov_y(const Float32 cameraFovY)
{
	fovY = cameraFovY;
}

Void Camera::update_camera_vectors()
{
	forward.x = cos(Math::to_radians(yaw)) * cos(Math::to_radians(pitch));
	forward.y = sin(Math::to_radians(pitch));
	forward.z = sin(Math::to_radians(yaw)) * cos(Math::to_radians(pitch));
	forward = Math::normalize(forward);

	right = Math::normalize(Math::cross(FVector3(0.0f, 1.0f, 0.0f), forward));
	up = Math::normalize(Math::cross(forward, right));
}