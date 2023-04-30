#pragma once
#include <map>

struct InputCommands
{
	// old

	bool forward;
	bool back;
	bool right;
	bool left;
	bool up;
	bool down;
	bool rotRight;
	bool rotLeft;

	// Arc Camera
	bool arcCameraModeToggle;
	bool arcCameraZoomIn;
	bool arcCameraZoomOut;

	void ResetState() 
	{
		ZeroMemory(this, sizeof(this));
	}
};

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
	// Flags
	OldActionsStart = Forward,
	OldActionsEnd = RotLeft,
	ArcCameraActionsStart = ArcCameraModeToggle,
	ArcCameraActionsEnd = ArcCameraModeToggle
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
		std::pair<MAP_PAIR>(Actions::ArcCameraZoomOut, MouseInput::WheelRollUp)
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
