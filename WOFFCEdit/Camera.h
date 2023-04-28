#pragma once
#define PI 3.14
#include "DisplayObject.h"

// namespaces
using namespace DirectX;
using namespace DirectX::SimpleMath;

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

	float RollRad() { return data.x * (PI / 180.0); }
	float PitchRad() { return data.z * (PI / 180.0); }
	float YawRad() { return data.y * (PI / 180.0); }

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
	~Camera() {};

	void Rotate(const Rotator& offsetAngle);
	void Move(const Vector3& offset, const bool relative = true);
	void SetFocus(std::shared_ptr<SceneObject> focus);
	void UnsetFocus();
	void Update();

	Matrix GetLookAtMatrix();
	const Vector3& GetPosition();

private:

	Vector3 m_camPosition;
	Rotator m_camOrientation;
	Vector3 m_camLookDirection;
	Vector3 m_camRight;
	Vector3 m_camUp;
	float m_moveSpeed;
	float m_camRotRate;

	std::shared_ptr<SceneObject> m_focusObject;

	void CalculateLookAtVector();
	void CalculateRightVector();
	void CalculateUpVector();

	void RecalculateRotation();
};