#include "Camera.h"

void Camera::Rotate(const Vector3& angle) 
{
	m_camOrientation += angle;
}