#include "Camera.h"
#include "MathFunctions.h"

Camera::Camera(Vector2D pos, float z) : defaultPosition(pos), position(pos), defaultZoom(z), zoom(z), targetZoom(z) {}

Vector2D Camera::ConvertWorldToScreen(const Vector2D& worldPos, const Vector2D& screenSize) const
{
	return Vector2D(
		(worldPos.x - position.x) * zoom + screenSize.x / 2,
		(worldPos.y - position.y) * zoom + screenSize.y / 2
	);
}

Vector2D Camera::ConvertScreenToWorld(const Vector2D& screenPos, const Vector2D& screenSize) const
{
	return Vector2D(
		(screenPos.x - screenSize.x / 2) / zoom + position.x,
		(screenPos.y - screenSize.y / 2) / zoom + position.y
	);
}

void Camera::Zoom(const Vector2D& screenPos, const Vector2D& screenSize, bool isZoomIn)
{
	Vector2D initialWorldPos = ConvertScreenToWorld(screenPos, screenSize);
	zoom *= isZoomIn ? zoomSpeed : 1.0f / zoomSpeed;
	Vector2D finalWorldPos = ConvertScreenToWorld(screenPos, screenSize);
	position = position + (initialWorldPos - finalWorldPos);
}

void Camera::Reset(const Vector2D& screenSize)
{
	position = defaultPosition;
	zoom = defaultZoom;
	targetZoom = defaultZoom;
}

void Camera::Update(double deltaTime)
{
	if (zoom != targetZoom)
	{
		float zoomStep = zoomLerpSpeed * deltaTime;

		if (std::abs(targetZoom - zoom) < 0.005f)
		{
			zoom = targetZoom; // Directly set zoom to target if close
		}
		else {
			// Interpolate zoom
			zoom = MathFunctions::ExpLerp(zoom, targetZoom, zoomStep);
		}

		// Clamp zoom
		zoom = MathFunctions::clamp(zoom, minZoom, maxZoom);
	}
}