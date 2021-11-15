#include "Camera.h"

Camera::Camera()
{
	movingUp = false;
	movingDown = false;
	movingRight = false;
	movingLeft = false;
	movingForward = false;
	movingBackward = false;
	orthoScale = 1;
}

void Camera::update()
{
	if (movingDown)
	{
		transform.position.y -= CAMERA_MOVE_SPEED;
	}
	else if (movingUp)
	{
		transform.position.y += CAMERA_MOVE_SPEED;
	}
	if (movingRight)
	{
		transform.position += CAMERA_MOVE_SPEED * transform.getRight();
	}
	else if (movingLeft)
	{
		transform.position -= CAMERA_MOVE_SPEED * transform.getRight();
	}
	if (movingForward)
	{
		transform.position -= CAMERA_MOVE_SPEED * transform.getForward();
	}
	else if (movingBackward)
	{
		transform.position += CAMERA_MOVE_SPEED * transform.getForward();
	}
}