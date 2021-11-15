#pragma once

#include "Transform.h"

// camera
#define CAMERA_MOVE_SPEED 1.2f
#define CAMERA_SCALE_SPEED 0.08f

struct Camera
{
	Transform transform;
	bool movingUp;
	bool movingDown;
	bool movingRight;
	bool movingLeft;
	bool movingForward;
	bool movingBackward;
	float orthoScale;

	Camera();

	void update();
};