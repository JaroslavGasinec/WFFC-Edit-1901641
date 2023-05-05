#pragma once
#include <map>
#include "ActionList.h"

enum class Actions : int
{

	#define LISTING(Action) Action,
	//Use macro to define all the actions
	ACTIONS(LISTING)
	#undef LISTING

	// Flags
	MaxNum,
	OldActionsStart = Forward,
	OldActionsEnd = RotLeft,
	CameraRelativeMovementStart = RotateCameraRelative,
	CameraRelativeMovementEnd = RotateCameraRelative,
	ArcCameraActionsStart = ArcCameraModeToggle,
	ArcCameraActionsEnd = ArcCameraSnapToZ,
	ObjectSelectionStart = SelectObject,
	ObjectSelectionEnd = MultiSelect,
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
		std::pair<MAP_PAIR>(Actions::RotLeft,'X'),
		std::pair<MAP_PAIR>(Actions::ArcCameraSnapToX,'X'),
		std::pair<MAP_PAIR>(Actions::ArcCameraSnapToY,'Y'),
		std::pair<MAP_PAIR>(Actions::ArcCameraSnapToZ,'Z'),
		std::pair<MAP_PAIR>(Actions::MultiSelect,VK_SHIFT),
		std::pair<MAP_PAIR>(Actions::ToggleEditMode,VK_TAB),
		std::pair<MAP_PAIR>(Actions::ToggleEditingAxisX,VK_NUMPAD1),
		std::pair<MAP_PAIR>(Actions::ToggleEditingAxisY,VK_NUMPAD2),
		std::pair<MAP_PAIR>(Actions::ToggleEditingAxisZ,VK_NUMPAD3),
		std::pair<MAP_PAIR>(Actions::ObjectMovePlus,VK_UP),
		std::pair<MAP_PAIR>(Actions::ObjectMoveMinus,VK_DOWN),
		std::pair<MAP_PAIR>(Actions::NormalizeBack,VK_BACK),
		std::pair<MAP_PAIR>(Actions::ToggleObjectRotate,'R'),
		std::pair<MAP_PAIR>(Actions::ToggleObjectMoveByMouse,'M'),
		std::pair<MAP_PAIR>(Actions::ObjectSizeUp,VK_ADD),
		std::pair<MAP_PAIR>(Actions::ObjectSizeDown,VK_SUBTRACT)
	};
	#undef MAP_PAIR

	#define MAP_PAIR Actions,MouseInput
	std::map<MAP_PAIR> mouseMapping = {
		std::pair<MAP_PAIR>(Actions::ArcCameraModeToggle, MouseInput::WheelButtonDown),
		std::pair<MAP_PAIR>(Actions::ArcCameraZoomIn, MouseInput::WheelRollDown),
		std::pair<MAP_PAIR>(Actions::ArcCameraZoomOut, MouseInput::WheelRollUp),
		std::pair<MAP_PAIR>(Actions::SelectObject, MouseInput::RButtonDown),
		std::pair<MAP_PAIR>(Actions::RotateCameraRelative, MouseInput::LButtonDown)
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
