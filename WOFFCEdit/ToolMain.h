#pragma once

#include <afxext.h>
#include "pch.h"
#include "Game.h"
#include "sqlite3.h"
#include "SceneObject.h"
#include "InputCommands.h"
#include <vector>
#include "EditModeDataTypes.h"

class ToolMain
{
public: //methods
	ToolMain();
	~ToolMain();

	//onAction - These are the interface to MFC
	int		getFirstIdFromCurrentSelection();										//returns the selection number of currently selected object so that It can be displayed.
	void	onActionInitialise(HWND handle, int width, int height);			//Passes through handle and height and width and initializes DirectX renderer and SQL LITE
	void	onActionFocusCamera();
	void	onActionLoad();													//load the current chunk
	afx_msg	void	onActionSave();											//save the current chunk
	afx_msg void	onActionSaveTerrain();									//save chunk geometry

	void	LoadMappingsFromConfig();

	void	Tick(MSG *msg);
	void	UpdateInput(MSG *msg);
	void	HandleInputCameraFocus();
	void	HandleInputSelectObject();
	void	HandleInputEditMode();

	//Editor mode handle inputs
	void EditMode_HandleInputAxisLocking();
	void EditMode_HandleInputScaling(std::vector<DisplayObject*> &selectedObjects);
	void EditMode_HandleInputRotating(std::vector<DisplayObject*>& selectedObjects);
	void EditMode_HandleInputMouseMoving(std::vector<DisplayObject*>& selectedObjects);
	void EditMode_HandleInputMoving(std::vector<DisplayObject*>& selectedObjects);


public:	//variables
	std::vector<SceneObject>    m_sceneGraph;	//our scenegraph storing all the objects in the current chunk
	ChunkObject					m_chunk;		//our landscape chunk
	int m_selectedObject;						//ID of current Selection

private:	//methods
	void	onContentAdded();
		
private:	//variables
	HWND	m_toolHandle;		//Handle to the  window
	Game	m_d3dRenderer;		//Instance of D3D rendering system for our tool
	InputCommands m_toolInputCommands;		//input commands that we want to use and possibly pass over to the renderer
	CRect	WindowRECT;		//Window area rectangle. 
	InputMapping m_inputMapping;
	char	m_keyArray[256];
	bool    m_mouseArray[(int)MouseInput::MouseDigitalInputEnd + 1];
	std::vector<int> m_selectedObjects;
	EditorMode m_editorMode;
	EditModeData m_editModeData;

	sqlite3 *m_databaseConnection;	//sqldatabase handle

	int m_width;		//dimensions passed to directX
	int m_height;
	int m_currentChunk;			//the current chunk of thedatabase that we are operating on.  Dictates loading and saving. 
};
