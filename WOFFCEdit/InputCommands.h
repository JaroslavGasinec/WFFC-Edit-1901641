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
	// Flags
	MaxNum,
	OldActionsStart = Forward,
	OldActionsEnd = RotLeft,
	ArcCameraActionsStart = ArcCameraModeToggle,
	ArcCameraActionsEnd = ArcCameraZoomOut,
	ObjectSelectionStart = SelectObject,
	ObjectSelectionEnd = SelectObject
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
	// old

	bool actionBuffer[(int)Actions::MaxNum];

	bool GetState(const Actions action)
	{
		return actionBuffer[(int)action];
	}

	void SetState(const Actions action)
	{
		actionBuffer[(int)action] = true;
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
		std::pair<MAP_PAIR>(Actions::RotLeft,'X')
	};
#undef MAP_PAIR

#define MAP_PAIR Actions,MouseInput
	std::map<MAP_PAIR> mouseMapping = {
		std::pair<MAP_PAIR>(Actions::ArcCameraModeToggle, MouseInput::WheelButtonDown),
		std::pair<MAP_PAIR>(Actions::ArcCameraZoomIn, MouseInput::WheelRollDown),
		std::pair<MAP_PAIR>(Actions::ArcCameraZoomOut, MouseInput::WheelRollUp),
		std::pair<MAP_PAIR>(Actions::SelectObject, MouseInput::LButtonDown),
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
