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
	m_focusObject = std::make_shared<SceneObject>();
	m_focusObject->posX = 0;
	m_focusObject->posY = 0;
	m_focusObject->posZ = 0;
	m_arcZoom = 1;
}

void Camera::Rotate(const Rotator& offsetRotation, const bool relative)
{
	if (relative)
		m_relativeRotation += offsetRotation;
	else
		m_rotation += offsetRotation;
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
	//JERRY TODO: PENDING OBJECT SELECTION
	//if (m_focusObject)
		CalculateOrientationFromFocus();
		//auto focusObjectPos = Vector3(m_focusObject->posX, m_focusObject->posY, m_focusObject->posY);
		auto focusObjectPos = Vector3(1, 1, 1);
		m_camPosition = focusObjectPos - (m_camForward * m_arcZoom);
	//else
		//CalculateOrientationVectors();
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

void Camera::CalculateOrientationFromFocus()
{
	//JERRY TODO: PENDING OBJECT SELECTION
	//m_camForward.x = m_focusObject->posX - m_camPosition.x;
	//m_camForward.y = m_focusObject->posY - m_camPosition.y;
	//m_camForward.z = m_focusObject->posZ - m_camPosition.z;
	m_camForward.x = 1 - m_camPosition.x;
	m_camForward.y = 1 - m_camPosition.y;
	m_camForward.z = 1 - m_camPosition.z;
	m_camForward.Normalize();

	// Decipher Yaw First, as rotations are applied in Roll, Pitch, Yaw order
	float yawRad = abs(atanf(m_camForward.z/m_camForward.x) * (180 / PI));
	Rotator::CleanupAngle(yawRad, m_camForward.x, -m_camForward.z);
	//Rotate clockwise
	Rotator yawRotation = Rotator(0, 0, yawRad);

	// Undo Yaw from forward vector
	Matrix vector = Matrix(
		m_camForward,
		Vector3(0,0,0),
		Vector3(0,0,0)
	);
	vector *= yawRotation.YawMatrix();

	// Decipher Pitch next
	float pitchRad = abs(atanf(vector._12 / vector._11) * (180 / PI));
	Rotator::CleanupAngle(pitchRad, vector._11, vector._12);
	//Rotate clockwise
	Rotator pitchRotation = Rotator(0, pitchRad, 0);

	// Apply the deciphered rotations to the up and right vectors
	auto rightUpMatrix = Matrix(
		Vector3(0, 0, 1),
		Vector3(0, 1, 0),
		Vector3(0, 0, 0)
	);

	pitchRotation.Pitch() = -pitchRotation.Pitch();
	rightUpMatrix *= pitchRotation.RotationMatrix();
	yawRotation.Yaw() = -yawRotation.Yaw();
	rightUpMatrix *= yawRotation.RotationMatrix();

	m_camRight.x = rightUpMatrix._11;
	m_camRight.y = rightUpMatrix._12;
	m_camRight.z = rightUpMatrix._13;
	m_camRight.Normalize();
	m_camUp.x = rightUpMatrix._21;
	m_camUp.y = rightUpMatrix._22;
	m_camUp.z = rightUpMatrix._23;
	m_camUp.Normalize();
}

void Camera::CalculateOrientationVectors() 
{	
	auto orientationMatrix = Matrix(
		Vector3(1, 0, 0),
		Vector3(0, 0, 1),
		Vector3(0, 1, 0)
	);

	orientationMatrix *= m_relativeRotation.RotationMatrix();
	orientationMatrix *= m_rotation.RotationMatrix();

	m_camForward.x = orientationMatrix._11;
	m_camForward.y = orientationMatrix._12;
	m_camForward.z = orientationMatrix._13;
	m_camForward.Normalize();
	m_camRight.x = orientationMatrix._21;
	m_camRight.y = orientationMatrix._22;
	m_camRight.z = orientationMatrix._23;
	m_camRight.Normalize();
	m_camUp.x = orientationMatrix._31;
	m_camUp.y = orientationMatrix._32;
	m_camUp.z = orientationMatrix._33;
	m_camUp.Normalize();
}

void Camera::CalculateRightVector()
{
	auto rightMatrix = Matrix(
		Vector3(0, 0, 1),
		Vector3(0, 0, 0),
		Vector3(0, 0, 0)
	);

	rightMatrix *= m_relativeRotation.RotationMatrix();
	rightMatrix *= m_rotation.RotationMatrix();

	m_camRight.x = rightMatrix._11;
	m_camRight.y = rightMatrix._12;
	m_camRight.z = rightMatrix._13;
	m_camRight.Normalize();
}

void Camera::CalculateUpVector()
{
	m_camRight.Cross(m_camForward, m_camUp);
}

void Camera::ArcZoomIn(const float offset)
{
	static const float minimumZoom = 0.2;
	m_arcZoom = std::max(minimumZoom, m_arcZoom + offset);
}

void Camera::ArcZoomReset()
{
	m_arcZoom = 1;
}
