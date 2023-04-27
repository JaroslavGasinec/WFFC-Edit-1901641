#pragma once
#include <map>

struct InputCommands
{
	// old

	bool forward;
	bool back;
	bool right;
	bool left;
	bool rotRight;
	bool rotLeft;

	// Arc Camera
	bool arcCameraModeToggle;
	bool cameraZoomIn;
	bool cameraZoomOut;

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
	RotRight,
	RotLeft,
	// Arc Camera
	ArcCameraModeToggle,
	ArcCameraModeEnd,
	CameraZoomIn,
	CameraZoomOut,
	// Flags
	OldActionsStart = Forward,
	OldActionsEnd = RotLeft,
	ArcCameraActionsStart = ArcCameraModeToggle,
	ArcCameraActionsEnd = CameraZoomOut,
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
		std::pair<MAP_PAIR>(Actions::RotRight,'E'),
		std::pair<MAP_PAIR>(Actions::RotLeft,'Q')
	};
#undef MAP_PAIR

#define MAP_PAIR Actions,MouseInput
	std::map<MAP_PAIR> mouseMapping = {
		std::pair<MAP_PAIR>(Actions::ArcCameraModeToggle, MouseInput::WheelButtonDown),
		std::pair<MAP_PAIR>(Actions::CameraZoomIn, MouseInput::WheelRollUp),
		std::pair<MAP_PAIR>(Actions::CameraZoomOut, MouseInput::WheelRollDown)
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
