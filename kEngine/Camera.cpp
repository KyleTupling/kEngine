#include "Camera.h"
#include "MathFunctions.h"

Camera::Camera(Vector2D pos, float z) : position(pos), zoom(z) {}

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

void Camera::Update(double deltaTime)
{
	if (zoom != targetZoom)
	{
		float zoomStep = zoomLerpSpeed * deltaTime;

		if (fabs(targetZoom - zoom) < zoomStep)
		{
			zoom = targetZoom; // Directly set zoom to target if close
		}
		else {
			// Linearly interpolate
			zoom += (targetZoom - zoom) * (zoomLerpSpeed * deltaTime);
		}

		// Clamp zoom
		zoom = MathFunctions::clamp(zoom, minZoom, maxZoom);
	}
}