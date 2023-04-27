#pragma once
#include "DisplayObject.h"

// typedef
typedef DirectX::SimpleMath::Vector3 Vector3;
typedef DirectX::SimpleMath::Matrix Matrix;

// forward declarations
class SceneObject;

struct Rotator 
{
	Rotator() {};
	Rotator(const float Roll, const float Pitch, const float Yaw) 
	{
		this->Roll() = Roll;
		this->Pitch() = Pitch;
		this->Yaw() = Yaw;
	}
	
	// x = Roll, Y = Yaw, Z = Pitch
	Vector3 data;

	float& Roll() { return data.x; }
	float& Pitch() { return data.z; }
	float& Yaw() { return data.y; }

	Rotator operator+ (const Rotator& rhs) 
	{
		Rotator out;
		out.data = data + rhs.data;
		return out;
	}

	Rotator operator- (const Rotator& rhs)
	{
		Rotator out;
		out.data = data - rhs.data;
		return out;
	}

	void operator+= (const Rotator& rhs)
	{
		data += rhs.data;
	}

	void operator-= (const Rotator& rhs)
	{
		data -= rhs.data;
	}
};

class Camera
{
public:
	Camera();
	~Camera();

	void Rotate(const Rotator& offsetAngle);
	void Move(const Vector3& offset, const bool relative = false);
	void SetFocus(std::shared_ptr<SceneObject> focus);
	void UnsetFocus();
	void Update();

	Matrix GetLookAtMatrix();

private:

	Vector3 m_camPosition;
	Rotator m_camOrientation;
	Vector3 m_camLookAt;
	Vector3 m_camLookDirection;
	Vector3 m_camRight;
	Vector3 m_camUp;
	float m_moveSpeed;
	float m_camRotRate;

	std::shared_ptr<SceneObject> m_focusOject;

	void CalculateLookAtVector();
	void CalculateRightVector();
	void CalculateUpVector();
};