#pragma once
#include <map>

enum class Actions : int
{
	// Old
	Forward = 0,
	Back,
	Right,
	Left,
	Up,
	Down,
	RotRight,
	RotLeft,
	// Arc Camera
	ArcCameraModeToggle,
	ArcCameraZoomIn,
	ArcCameraZoomOut,
	// Object Selection
	SelectObject,
	DeselectObject,
	// Selected Object(s) Alteration
	ToggleEditMode,
	ToggleEditingAxisX,
	ToggleEditingAxisY,
	ToggleEditingAxisZ,
	ToggleObjectMoveByMouse,
	ObjectMoveToMouse,
	ObjectSizeUp,
	ObjectSizeDown,
	ToggleObjectRotate,
	ObjectMovePlus,
	ObjectMoveMinus,
	// Flags
	MaxNum,
	OldActionsStart = Forward,
	OldActionsEnd = RotLeft,
	ArcCameraActionsStart = ArcCameraModeToggle,
	ArcCameraActionsEnd = ArcCameraZoomOut,
	ObjectSelectionStart = SelectObject,
	ObjectSelectionEnd = DeselectObject,
	ObjectAlterationStart = ToggleEditMode,
	ObjectAlterationEnd = ObjectMoveMinus
};

enum class MouseInput : int
{
	LButtonDown = 0,
	RButtonDown,
	WheelButtonDown,
	WheelRollUp,
	WheelRollDown,
	MouseMoveUp,
	MouseMoveDown,
	// Flags
	MouseDigitalInputEnd = WheelRollDown
};

struct InputCommands
{
	bool m_actionBuffer[(int)Actions::MaxNum];
	long m_mousePos[2] = { 0,0 };
	long m_mouseDelta[2] = { 0,0 };

	bool GetState(const Actions action, const bool consume = true)
	{
		bool out = m_actionBuffer[(int)action];
		if (consume)
			m_actionBuffer[(int)action] = false;

		return out;
	}

	void SetState(const Actions action, const bool state = true)
	{
		m_actionBuffer[(int)action] = state;
	}

	void ResetState()
	{
		ZeroMemory(this, sizeof(this));
	}
};

struct InputMapping 
{
#define MAP_PAIR Actions,char
	std::map<MAP_PAIR> keyMapping = {
		std::pair<MAP_PAIR>(Actions::Forward,'W'),
		std::pair<MAP_PAIR>(Actions::Back,'S'),
		std::pair<MAP_PAIR>(Actions::Right,'D'),
		std::pair<MAP_PAIR>(Actions::Left,'A'),
		std::pair<MAP_PAIR>(Actions::Up,'Q'),
		std::pair<MAP_PAIR>(Actions::Down,'E'),
		std::pair<MAP_PAIR>(Actions::RotRight,'Z'),
		std::pair<MAP_PAIR>(Actions::ToggleEditMode,'`'),
		std::pair<MAP_PAIR>(Actions::ToggleEditingAxisX,'7'),
		std::pair<MAP_PAIR>(Actions::ToggleEditingAxisY,'1'),
		std::pair<MAP_PAIR>(Actions::ToggleEditingAxisZ,'3'),
		std::pair<MAP_PAIR>(Actions::ToggleObjectRotate,'R'),
		std::pair<MAP_PAIR>(Actions::ToggleObjectMoveByMouse,'M'),
		std::pair<MAP_PAIR>(Actions::ObjectSizeUp,'+'),
		std::pair<MAP_PAIR>(Actions::ObjectSizeDown,'-')
	};
#undef MAP_PAIR

#define MAP_PAIR Actions,MouseInput
	std::map<MAP_PAIR> mouseMapping = {
		std::pair<MAP_PAIR>(Actions::ArcCameraModeToggle, MouseInput::WheelButtonDown),
		std::pair<MAP_PAIR>(Actions::ArcCameraZoomIn, MouseInput::WheelRollDown),
		std::pair<MAP_PAIR>(Actions::ArcCameraZoomOut, MouseInput::WheelRollUp),
		std::pair<MAP_PAIR>(Actions::SelectObject, MouseInput::LButtonDown),
		std::pair<MAP_PAIR>(Actions::DeselectObject, MouseInput::RButtonDown),
	};
#undef MAP_PAIR

	struct MappingResult
	{
		int charId = -1;
		bool isMouse = false;
	};

	MappingResult GetMapping(const Actions action)
	{
		MappingResult out;

		if (keyMapping.find(action) != keyMapping.end())
		{
			out.charId = keyMapping.at(action);
			return out;
		}
		else if (mouseMapping.find(action) != mouseMapping.end())
		{
			out.charId = (int)mouseMapping.at(action);
			out.isMouse = true;
			return out;
		}

		return out;
	}
};
