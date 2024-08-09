#pragma once

#include "Vector2D.h"

class Camera
{
public:
	Vector2D defaultPosition;
	Vector2D position;
	float moveSpeed = 5.0f;

	float defaultZoom;
	float zoom;
	float zoomSpeed = 1.1f;
	float zoomDelta = 0.0f;
	float minZoom = 0.5f;
	float maxZoom = 4.0f;

	float targetZoom;
	float zoomLerpSpeed = 5.0f;

	Camera(Vector2D pos = Vector2D(0, 0), float z = 1.0f);

	// Converts from world coordinates to screen coordinates
	Vector2D ConvertWorldToScreen(const Vector2D& worldPos, const Vector2D& screenSize) const;
	// Converts from screen coordinates to world coordinates
	Vector2D ConvertScreenToWorld(const Vector2D& screenPos, const Vector2D& screenSize) const;

	// Zooms the camera around a given screen position
	void Zoom(const Vector2D& screenPos, const Vector2D& screenSize, bool isZoomIn);

	// Resets camera to initial position and zoom
	void Reset(const Vector2D& screenSize);

	void Update(double deltaTime);
};

