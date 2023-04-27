#include "Camera.h"
#include "SceneObject.h"

Camera::Camera()
{
	m_camPosition = Vector3(0, 0, 0);
	m_camOrientation = Rotator(0, 0, 0);
	m_camLookAt = Vector3(1, 0, 0);
	m_camLookDirection = Vector3(1, 0, 0);
	m_camRight = Vector3(0, 0, 0);
	m_camUp = Vector3(0, 0, 0);
	m_moveSpeed = 5;
	m_camRotRate = 10;
}

void Camera::Rotate(const Rotator& offsetAngle)
{
	m_camOrientation += offsetAngle;
}

void Camera::Move(const Vector3& offset, const bool relative)
{
	if (relative)
	{
		Vector3 final;
		final = offset.x * m_camLookDirection;
		final += offset.z * m_camRight;
		final += offset.y * m_camUp;
		m_camPosition += final * m_moveSpeed;
		return;
	}

	m_camPosition += offset * m_moveSpeed;
}

void Camera::CalculateLookAtVector() 
{	
	if (!m_focusOject)
	{
		const float yaw = m_camOrientation.Yaw();
		const float pitch = m_camOrientation.Pitch();

		m_camLookDirection.x = cos(yaw) * cos(pitch);
		m_camLookDirection.z = sin(yaw) * cos(pitch);
		m_camLookDirection.y = sin(pitch);
		m_camLookDirection.Normalize();
		return;
	}

	m_camLookDirection.x = m_focusOject->posX - m_camPosition.x;
	m_camLookDirection.y = m_focusOject->posX - m_camPosition.y;
	m_camLookDirection.z = m_focusOject->posX - m_camPosition.z;
	m_camLookDirection.Normalize();
}

void Camera::CalculateRightVector()
{
	const float roll = m_camOrientation.Roll();
	const float yaw = m_camOrientation.Yaw();
	const float pitch = m_camOrientation.Pitch();

	if (!m_focusOject)
	{
		m_camRight.x = (-cos(yaw) * sin(pitch) * sin(roll)) - (sin(yaw) * cos(roll));
		m_camRight.z = (-sin(yaw) * sin(pitch) * sin(roll)) + (cos(yaw) * cos(roll));
		m_camRight.y = cos(pitch) * sin(roll);
		m_camRight *= -1;
		m_camRight.Normalize();
	}
}

void Camera::CalculateUpVector()
{
	m_camRight.Cross(m_camLookDirection, m_camUp);
}
