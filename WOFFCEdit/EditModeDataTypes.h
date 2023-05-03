#pragma once
enum class EditorMode : int
{
	Default,
	Edit
};

struct EditorModeData
{
	enum class Axis : int
	{
		X,
		Y,
		Z,
		Num
	};

	float m_scalingStep = 0.01f;
	float m_moveStep = 2.f;
	float m_rotateStep = 1.f;
	bool m_rotating = false;
	bool m_mouseMoving = false;

	bool transformAxis[(int)Axis::Num] = { true, true, true };

	void Reset()
	{
		for (int i = 0; i < (int)Axis::Num; i++)
			transformAxis[i] = true;

		m_rotating = false;
		m_mouseMoving = false;
	}

	void ToggleEditAxis(const Axis axis)
	{
		transformAxis[(int)axis] = !transformAxis[(int)axis];
	}

	bool IsAxisUnlocked(const Axis axis) const 
	{
		return transformAxis[(int)axis];
	}
};