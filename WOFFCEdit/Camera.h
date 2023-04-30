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

	float C_Roll() const { return data.x; }
	float C_Pitch() const { return data.z; }
	float C_Yaw() const { return data.y; }

	float RollRad() const { return data.x * (PI / 180.0); }
	float PitchRad() const { return data.z * (PI / 180.0); }
	float YawRad() const { return data.y * (PI / 180.0); }

	Matrix RollMatrix() const
	{
		return Matrix(
			Vector3(1.f, 0.f, 0.f),
			Vector3(0.f, cosf(RollRad()), -sinf(RollRad())),
			Vector3(0.f, sinf(RollRad()), cosf(RollRad()))
		);
	}

	Matrix PitchMatrix() const
	{
		return Matrix(
			Vector3(cosf(PitchRad()), -sinf(PitchRad()), 0.f),
			Vector3(sinf(PitchRad()), cosf(PitchRad()), 0.f),
			Vector3(0.f, 0.f, 1.f)
		);
	}

	Matrix YawMatrix() const
	{
		return Matrix(
			Vector3(cosf(YawRad()), 0.f, sinf(YawRad())),
			Vector3(0.f, 1.f, 0.f),
			Vector3(-sinf(YawRad()), 0.f, cosf(YawRad()))
		);
	}

	Matrix RotationMatrix() const
	{
		Matrix out = RollMatrix();
		out *= PitchMatrix();
		out *= YawMatrix();
		return out;
	}

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
	
	static Matrix RotationAboutAxis(const float a, Vector3 v)
	{
		const float cosA = cosf(a);
		const float sinA = sinf(a);
		v.Normalize();

		Matrix out = Matrix(
			Vector3(cosA + pow(v.x, 2) * (1 - cosA), v.x * v.y * (1 - cosA) - v.z * sinA, v.x * v.z * (1 - cosA) + v.y * sinA),
			Vector3(v.y * v.x * (1 - cosA) + v.z * sinA, cosA + pow(v.y, 2) * (1 - cosA), v.y * v.z * (1 - cosA) - v.x * sinA),
			Vector3(v.z * v.x * (1 - cosA) - v.y * sinA, v.z * v.y * (1 - cosA) + v.x * sinA, cosA + pow(v.z, 2) * (1 - cosA))
		);

		return out;
	}

	// x = forward, y = yaw, z = right
	static Rotator RotatorFromVector(const Vector3& v)
	{
		Rotator out;

		out.Roll() = 0;
		out.Pitch() = asinf(v.y / v.Length()) * (180.0f / PI);
		out.Yaw() = atanf(-v.z / v.x) * (180.0f / PI);
		float shadowPitch = atanf(v.y / v.x) * (180.0f / PI);

		static auto AngleCleanup = [&](float& angle, const float& x, const float& y)
		{
			// default comparisons
			if (x == 0)
			{
				if (y > 0)
					angle = 90.f;
				else
					angle = 270.f;

				return;
			}

			if (y == 0)
			{
				if (x > 0)
					angle = 0.f;
				else
					angle = 180.f;

				return;
			}

			// quadrant based cleanup
			int quadrant = 0;
			if (x > 0 && y > 0)
				quadrant = 1;
			else if (x < 0 && y > 0)
				quadrant = 2;
			else if (x < 0 && y < 0)
				quadrant = 3;
			else
				quadrant = 4;

			switch(quadrant)
			{
				case 0:
					angle = 0;
					return;
				case 1:
					return;
				case 2:
					angle = 180 - angle;
					return;
				case 3:
					angle += 180;
					return;
				case 4:
					angle = 360 - angle;
					return;
			}
		};

		AngleCleanup(shadowPitch, v.x, v.y);
		const float pitchXAxis = sqrt(v.x * v.x + v.z * v.z) * ((shadowPitch > 90 && shadowPitch < 270) ? -1 : 1);

		out.Pitch() = fabsf(out.C_Pitch());
		out.Yaw() = fabsf(out.C_Yaw());

		AngleCleanup(out.Pitch(), pitchXAxis, v.y);
		AngleCleanup(out.Yaw(), v.x, -v.z);

		return out;
	}
};

class Camera
{
public:
	Camera();
	~Camera() {};

	void Rotate(const Rotator& offsetRotation, const bool relative = true);
	void Rotate(const Vector3& relativeDirection);
	void Move(const Vector3& offset, const bool relative = true);
	void SetFocus(std::shared_ptr<SceneObject> focus);
	void UnsetFocus();
	void Update();

	Matrix GetLookAtMatrix();
	const Vector3& GetPosition();

private:

	Vector3 m_camPosition;
	Vector3 m_camForward;
	Vector3 m_camRight;
	Vector3 m_camUp;
	Matrix m_camRotation;
	float m_moveSpeed;
	float m_camRotRate;

	std::shared_ptr<SceneObject> m_focusObject;

	void ForwardToFocus();
	void CalculateOrientationVectors();
	void CalculateRightVector();
	void CalculateUpVector();

	Matrix RotationFromForward();

public:
	Rotator RotatorFromForward();
};