#pragma once

#include "DisplayObject.h"
#include "InputCommands.h"
#include "Rotator.h"

// namespaces
using namespace DirectX;
using namespace DirectX::SimpleMath;

// forward declarations
class SceneObject;
class Camera
{
public:
	Camera();
	~Camera() = default;

	void Rotate(const Rotator& offsetRotation, const bool relative = false);
	void Move(const Vector3& offset, const bool relative = true);
	void ArcZoomIn(const float offset);
	void ArcZoomReset();
	void SetFocus(DisplayObject* focus);
	void UnsetFocus();
	bool HasFocus();
	void Update();
	void HandleInput(const float deltaTime, InputCommands& input);

	Matrix GetLookAtMatrix();
	const Vector3& GetPosition();

//private:
public:
	Vector3 m_camPosition;
	Vector3 m_camForward;
	Vector3 m_camRight;
	Vector3 m_camUp;
	Rotator m_relativeRotation;
	Rotator m_rotation;
	float m_camMoveSpeed;
	float m_camZoomSpeed;
	float m_camRotRate;
	float m_arcZoom;

	DisplayObject* m_focusObject;

	void CalculateOrientationFromFocus();
	void CalculateOrientationVectors();
	void CalculateRightVector();
	void CalculateUpVector();
};
