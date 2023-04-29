#include "Camera.h"
#include "SceneObject.h"

Camera::Camera()
{
	m_camPosition = Vector3(0, 0, 0);
	m_camForward = Vector3(1, 0, 0);
	m_camRight = Vector3(0, 0, 1);
	m_camUp = Vector3(0, 1, 0);
	m_moveSpeed = 5;
	m_camRotRate = 10;
}

void Camera::Rotate(const Rotator& offsetRotation, const bool relative)
{
	if (relative)
		m_camRotation = offsetRotation.RotationMatrix() * m_camRotation;
	else
		m_camRotation *= offsetRotation.RotationMatrix();
}

void Camera::Rotate(const Vector3& relativeDirection)
{
	Vector3 finalOffset = m_camRight * relativeDirection.z + m_camUp * relativeDirection.y;

	m_camForward += finalOffset;
	m_camForward.Normalize();

	RotationFromForward();

	CalculateRightVector();
	CalculateOrientationVectors();
}

void Camera::Move(const Vector3& offset, const bool relative)
{
	if (relative)
	{
		Vector3 final;
		final = offset.x * m_camForward;
		final += offset.z * m_camRight;
		final += offset.y * m_camUp;
		m_camPosition += final;
		return;
	}

	m_camPosition += offset;
}

void Camera::SetFocus(std::shared_ptr<SceneObject> focus)
{
	if (focus)
		m_focusObject = focus;
}

void Camera::UnsetFocus()
{
	m_focusObject = nullptr;
}

void Camera::Update()
{
	CalculateOrientationVectors();
	//if (m_focusObject)
		//RotationFromForward();

	//CalculateRightVector();
	//CalculateUpVector();
}

Matrix Camera::GetLookAtMatrix()
{
	auto m_camLookAt = m_camPosition + m_camForward;
	return Matrix::CreateLookAt(m_camPosition, m_camLookAt, m_camUp);
}

const Vector3& Camera::GetPosition()
{
	return m_camPosition;
}

void Camera::CalculateOrientationVectors() 
{	
	if (!m_focusObject)
	{
		auto oriantationMatrix = Matrix(
			Vector3(1, 0, 0),
			Vector3(0, 0, 1),
			Vector3(0, 1, 0)
		);

		oriantationMatrix *= m_camRotation;

		m_camForward.x = oriantationMatrix._11;
		m_camForward.y = oriantationMatrix._12;
		m_camForward.z = oriantationMatrix._13;
		m_camForward.Normalize();
		m_camRight.x = oriantationMatrix._21;
		m_camRight.y = oriantationMatrix._22;
		m_camRight.z = oriantationMatrix._23;
		m_camRight.Normalize();
		m_camUp.x = oriantationMatrix._31;
		m_camUp.y = oriantationMatrix._32;
		m_camUp.z = oriantationMatrix._33;
		m_camUp.Normalize();
		return;
	}

	m_camForward.x = m_focusObject->posX - m_camPosition.x;
	m_camForward.y = m_focusObject->posY - m_camPosition.y;
	m_camForward.z = m_focusObject->posZ - m_camPosition.z;
	m_camForward.Normalize();
}

void Camera::CalculateRightVector()
{
	//if (!m_focusObject)
	//{
		auto rightMatrix = Matrix(
			Vector3(0, 0, 1),
			Vector3(0, 0, 0),
			Vector3(0, 0, 0)
		);

		rightMatrix *= m_camRotation;

		m_camRight.x = rightMatrix._11;
		m_camRight.y = rightMatrix._12;
		m_camRight.z = rightMatrix._13;
		m_camRight.Normalize();
		return;
	//}
}

void Camera::CalculateUpVector()
{
	m_camRight.Cross(m_camForward, m_camUp);
}

Matrix Camera::RotationFromForward()
{
	return RotatorFromForward().RotationMatrix();
}

Rotator Camera::RotatorFromForward()
{
	Rotator delta;

	delta.Roll() = 0;
	delta.Pitch() = atanf(m_camForward.y / m_camForward.x) * (180.0f / PI);
	delta.Yaw() = atanf(m_camForward.z / m_camForward.x) * (180.0f / PI);

	// Pitch alterations
	if (m_camForward.z < 0)
		delta.Pitch() = 180 - delta.Pitch();

	if (m_camForward.x < 0)
		delta.Pitch() *= -1;

	// Yaw alterations
	if (m_camForward.x < 0)
		delta.Yaw() = 180 - delta.Yaw();

	if (m_camForward.y < 0)
		delta.Yaw() *= -1;

	return delta;
}
