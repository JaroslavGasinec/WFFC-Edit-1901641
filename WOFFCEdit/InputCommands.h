#pragma once

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
	bool arcCameraMode;
	long cameraRelativeUp;
	long cameraRelativeDown;
	long cameraRelativeLeft;
	long cameraRelativeRight;
	bool cameraZoomIn;
	bool cameraZoomOut;

	void ResetState() 
	{
		ZeroMemory(this, sizeof(this));
	}
};
