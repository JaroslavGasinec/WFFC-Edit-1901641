#include "Camera.h"
#include "SceneObject.h"

Camera::Camera()
{
	m_camPosition = Vector3(0, 0, 0);
	m_camForward = Vector3(1, 0, 0);
	m_camRight = Vector3(0, 0, 1);
	m_camUp = Vector3(0, 1, 0);
	m_camMoveSpeed = 5;
	m_camZoomSpeed = 5;
	m_camRotRate = 10;
	m_focusObject = nullptr;
	m_arcZoom = 1;
	m_sensitivity = 0.5;
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

void Camera::SetFocus(DisplayObject* focus)
{
	if (focus)
		m_focusObject = focus;
}

void Camera::UnsetFocus()
{
	m_focusObject = nullptr;
}

bool Camera::HasFocus()
{
	return m_focusObject != nullptr;
}

void Camera::Update()
{
	if (m_focusObject)
	{
		CalculateOrientationFromFocus();
		auto focusObjectPos = m_focusObject->m_position;
		m_camPosition = focusObjectPos - (m_camForward * m_arcZoom);
	}
	else
	{
		CalculateOrientationVectors();
	}
}

void Camera::HandleInput(const float deltaTime, InputCommands& input)
{
	if (!m_focusObject)
	{
		// Key-based rotation
		if (input.GetState(Actions::RotRight, false))
			Rotate(Rotator(0, 0, -m_camRotRate * deltaTime));

		if (input.GetState(Actions::RotLeft, false))
			Rotate(Rotator(0, 0, m_camRotRate * deltaTime));

		// Handle non-arc mode movement
		if (input.GetState(Actions::Forward, false))
			Move(Vector3(m_camMoveSpeed * deltaTime, 0, 0));

		if (input.GetState(Actions::Back, false))
			Move(Vector3(-m_camMoveSpeed * deltaTime, 0, 0));

		if (input.GetState(Actions::RotateCameraRelative, false))
			Rotate(Rotator(
				0,
				input.m_mouseDelta[1] * m_sensitivity,
				input.m_mouseDelta[0] * m_sensitivity
				));
	}
	else
	{
		if (input.GetState(Actions::ArcCameraZoomIn))
			ArcZoomIn(m_camZoomSpeed * deltaTime);

		if (input.GetState(Actions::ArcCameraZoomOut))
			ArcZoomIn(-m_camZoomSpeed * deltaTime);

		if (input.GetState(Actions::ArcCameraSnapToX))
			m_camPosition = m_focusObject->m_position - Vector3(1, 0, 0) * m_arcZoom;

		if (input.GetState(Actions::ArcCameraSnapToY))
			m_camPosition = m_focusObject->m_position - Vector3(0, 1, 0) * m_arcZoom;

		if (input.GetState(Actions::ArcCameraSnapToZ))
			m_camPosition = m_focusObject->m_position - Vector3(0, 0, 1) * m_arcZoom;
	}

	// Handle camera movement
	if (input.GetState(Actions::Right, false))
		Move(Vector3(0, 0, m_camMoveSpeed * deltaTime));

	if (input.GetState(Actions::Left, false))
		Move(Vector3(0, 0, -m_camMoveSpeed * deltaTime));

	if (input.GetState(Actions::Up, false))
		Move(Vector3(0, m_camMoveSpeed * deltaTime, 0));

	if (input.GetState(Actions::Down, false))
		Move(Vector3(0, -m_camMoveSpeed * deltaTime, 0));
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
	m_camForward = m_focusObject->m_position - m_camPosition;
	m_camForward.Normalize();

	// Decipher Yaw First, as camera rotations are applied in Roll, Pitch, Yaw order
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
