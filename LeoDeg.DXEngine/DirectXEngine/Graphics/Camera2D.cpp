#include "Camera2D.h"

DXEngine::Camera2D::Camera2D ()
{
	Transform.updateViewMatrix = true;
	Transform.isObject2D = true;
}
