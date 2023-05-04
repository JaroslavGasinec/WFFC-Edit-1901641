#pragma once
enum class EditorMode : int
{
	Default,
	Edit
};

struct ModeData
{
	EditorMode GetType() const { return m_type; }

protected:
	EditorMode m_type = EditorMode::Default;
};

struct EditModeData : ModeData
{
	enum class Axis : int
	{
		X,
		Y,
		Z,
		Num
	};

	const float m_scalingStep;
	const float m_moveStep;
	const float m_rotateStep;
	bool m_rotating = false;
	bool m_mouseMoving = false;
	bool transformAxis[(int)Axis::Num] = { true, true, true };

	EditModeData() :
	m_scalingStep(0.01f),
	m_moveStep(2.f),
	m_rotateStep(50.0f)
	{
		bool m_rotating = false;
		bool m_mouseMoving = false;
		bool transformAxis[(int)Axis::Num] = { true, true, true };
		m_type = EditorMode::Edit;
	}

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

	int NumOfEditingAxes() 
	{
		int out = 0;
		for (int i = 0; i < 3; i++)
			if (transformAxis[i])
				out++;

		return out;
	}
};