#pragma once
#include "DisplayObject.h"

// typedef
typedef DirectX::SimpleMath::Vector3 Vector3;
typedef DirectX::SimpleMath::Matrix Matrix;

// forward declarations
class SceneObject;

class Camera
{
public:
	Camera();
	~Camera();

	void Rotate(const Vector3& angle);
	void Move(const Vector3& offset, const bool relative = false);
	void SetFocus(std::shared_ptr<SceneObject> focus);
	void UnsetFocus();

	Matrix GetLookAtMatrix();

private:

	Vector3 m_camPosition;
	Vector3 m_camOrientation;
	Vector3 m_camLookAt;
	Vector3 m_camLookDirection;
	Vector3 m_camRight;
	Vector3 m_camUp;
	float m_moveSpeed;
	float m_camRotRate;
	bool m_arcMode;

	std::shared_ptr<SceneObject> m_focusOject;

	void CalculateLookAtVector();
};