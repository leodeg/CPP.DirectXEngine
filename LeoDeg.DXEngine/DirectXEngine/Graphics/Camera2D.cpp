#include "Camera2D.h"

DXEngine::Camera2D::Camera2D ()
{
	Transform.updateViewMatrix = true;
	Transform.is2D = true;
}
