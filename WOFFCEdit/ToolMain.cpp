#include "ToolMain.h"
#include "resource.h"
#include <vector>
#include <sstream>
#include <algorithm>

//
//ToolMain Class
ToolMain::ToolMain()
{

	m_currentChunk = 0;		//default value
	m_selectedObject = 0;	//initial selection ID
	m_sceneGraph.clear();	//clear the vector for the scenegraph
	m_databaseConnection = NULL;
	m_editorMode = EditorMode::Default;

	//zero input commands
	m_toolInputCommands.ResetState();	
}


ToolMain::~ToolMain()
{
	sqlite3_close(m_databaseConnection);		//close the database connection
}


int ToolMain::getFirstIdFromCurrentSelection()
{
	if (m_selectedObjects.size() > 0)
		return m_selectedObjects[0];
	
	return -1;
}

void ToolMain::onActionInitialise(HWND handle, int width, int height)
{
	//window size, handle etc for directX
	m_width		= width;
	m_height	= height;
	
	m_d3dRenderer.Initialize(handle, m_width, m_height);

	//database connection establish
	int rc;
	rc = sqlite3_open_v2("database/test.db",&m_databaseConnection, SQLITE_OPEN_READWRITE, NULL);

	if (rc) 
	{
		TRACE("Can't open database");
		//if the database cant open. Perhaps a more catastrophic error would be better here
	}
	else 
	{
		TRACE("Opened database successfully");
	}

	onActionLoad();
}

void ToolMain::onActionLoad()
{
	//load current chunk and objects into lists
	if (!m_sceneGraph.empty())		//is the vector empty
	{
		m_sceneGraph.clear();		//if not, empty it
	}

	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	sqlite3_stmt *pResultsChunk;

	//OBJECTS IN THE WORLD
	//prepare SQL Text
	sqlCommand = "SELECT * from Objects";				//sql command which will return all records from the objects table.
	//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0 );
	
	//loop for each row in results until there are no more rows.  ie for every row in the results. We create and object
	while (sqlite3_step(pResults) == SQLITE_ROW)
	{	
		SceneObject newSceneObject;
		newSceneObject.ID = sqlite3_column_int(pResults, 0);
		newSceneObject.chunk_ID = sqlite3_column_int(pResults, 1);
		newSceneObject.model_path		= reinterpret_cast<const char*>(sqlite3_column_text(pResults, 2));
		newSceneObject.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 3));
		newSceneObject.posX = sqlite3_column_double(pResults, 4);
		newSceneObject.posY = sqlite3_column_double(pResults, 5);
		newSceneObject.posZ = sqlite3_column_double(pResults, 6);
		newSceneObject.rotX = sqlite3_column_double(pResults, 7);
		newSceneObject.rotY = sqlite3_column_double(pResults, 8);
		newSceneObject.rotZ = sqlite3_column_double(pResults, 9);
		newSceneObject.scaX = sqlite3_column_double(pResults, 10);
		newSceneObject.scaY = sqlite3_column_double(pResults, 11);
		newSceneObject.scaZ = sqlite3_column_double(pResults, 12);
		newSceneObject.render = sqlite3_column_int(pResults, 13);
		newSceneObject.collision = sqlite3_column_int(pResults, 14);
		newSceneObject.collision_mesh = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 15));
		newSceneObject.collectable = sqlite3_column_int(pResults, 16);
		newSceneObject.destructable = sqlite3_column_int(pResults, 17);
		newSceneObject.health_amount = sqlite3_column_int(pResults, 18);
		newSceneObject.editor_render = sqlite3_column_int(pResults, 19);
		newSceneObject.editor_texture_vis = sqlite3_column_int(pResults, 20);
		newSceneObject.editor_normals_vis = sqlite3_column_int(pResults, 21);
		newSceneObject.editor_collision_vis = sqlite3_column_int(pResults, 22);
		newSceneObject.editor_pivot_vis = sqlite3_column_int(pResults, 23);
		newSceneObject.pivotX = sqlite3_column_double(pResults, 24);
		newSceneObject.pivotY = sqlite3_column_double(pResults, 25);
		newSceneObject.pivotZ = sqlite3_column_double(pResults, 26);
		newSceneObject.snapToGround = sqlite3_column_int(pResults, 27);
		newSceneObject.AINode = sqlite3_column_int(pResults, 28);
		newSceneObject.audio_path = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 29));
		newSceneObject.volume = sqlite3_column_double(pResults, 30);
		newSceneObject.pitch = sqlite3_column_double(pResults, 31);
		newSceneObject.pan = sqlite3_column_int(pResults, 32);
		newSceneObject.one_shot = sqlite3_column_int(pResults, 33);
		newSceneObject.play_on_init = sqlite3_column_int(pResults, 34);
		newSceneObject.play_in_editor = sqlite3_column_int(pResults, 35);
		newSceneObject.min_dist = sqlite3_column_double(pResults, 36);
		newSceneObject.max_dist = sqlite3_column_double(pResults, 37);
		newSceneObject.camera = sqlite3_column_int(pResults, 38);
		newSceneObject.path_node = sqlite3_column_int(pResults, 39);
		newSceneObject.path_node_start = sqlite3_column_int(pResults, 40);
		newSceneObject.path_node_end = sqlite3_column_int(pResults, 41);
		newSceneObject.parent_id = sqlite3_column_int(pResults, 42);
		newSceneObject.editor_wireframe = sqlite3_column_int(pResults, 43);
		newSceneObject.name = reinterpret_cast<const char*>(sqlite3_column_text(pResults, 44));

		newSceneObject.light_type = sqlite3_column_int(pResults, 45);
		newSceneObject.light_diffuse_r = sqlite3_column_double(pResults, 46);
		newSceneObject.light_diffuse_g = sqlite3_column_double(pResults, 47);
		newSceneObject.light_diffuse_b = sqlite3_column_double(pResults, 48);
		newSceneObject.light_specular_r = sqlite3_column_double(pResults, 49);
		newSceneObject.light_specular_g = sqlite3_column_double(pResults, 50);
		newSceneObject.light_specular_b = sqlite3_column_double(pResults, 51);
		newSceneObject.light_spot_cutoff = sqlite3_column_double(pResults, 52);
		newSceneObject.light_constant = sqlite3_column_double(pResults, 53);
		newSceneObject.light_linear = sqlite3_column_double(pResults, 54);
		newSceneObject.light_quadratic = sqlite3_column_double(pResults, 55);

		//send completed object to scenegraph
		m_sceneGraph.push_back(newSceneObject);
	}

	//THE WORLD CHUNK
	//prepare SQL Text
	sqlCommand = "SELECT * from Chunks";				//sql command which will return all records from  chunks table. There is only one tho.
														//Send Command and fill result object
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResultsChunk, 0);

	sqlite3_step(pResultsChunk);
	m_chunk.ID = sqlite3_column_int(pResultsChunk, 0);
	m_chunk.name = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 1));
	m_chunk.chunk_x_size_metres = sqlite3_column_int(pResultsChunk, 2);
	m_chunk.chunk_y_size_metres = sqlite3_column_int(pResultsChunk, 3);
	m_chunk.chunk_base_resolution = sqlite3_column_int(pResultsChunk, 4);
	m_chunk.heightmap_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 5));
	m_chunk.tex_diffuse_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 6));
	m_chunk.tex_splat_alpha_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 7));
	m_chunk.tex_splat_1_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 8));
	m_chunk.tex_splat_2_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 9));
	m_chunk.tex_splat_3_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 10));
	m_chunk.tex_splat_4_path = reinterpret_cast<const char*>(sqlite3_column_text(pResultsChunk, 11));
	m_chunk.render_wireframe = sqlite3_column_int(pResultsChunk, 12);
	m_chunk.render_normals = sqlite3_column_int(pResultsChunk, 13);
	m_chunk.tex_diffuse_tiling = sqlite3_column_int(pResultsChunk, 14);
	m_chunk.tex_splat_1_tiling = sqlite3_column_int(pResultsChunk, 15);
	m_chunk.tex_splat_2_tiling = sqlite3_column_int(pResultsChunk, 16);
	m_chunk.tex_splat_3_tiling = sqlite3_column_int(pResultsChunk, 17);
	m_chunk.tex_splat_4_tiling = sqlite3_column_int(pResultsChunk, 18);

	//Process REsults into renderable
	m_d3dRenderer.BuildDisplayList(&m_sceneGraph);
	//build the renderable chunk 
	m_d3dRenderer.BuildDisplayChunk(&m_chunk);

}

void ToolMain::onActionSave()
{
	//Get all the changes from the viewport into the scenegraph
	m_d3dRenderer.CommitDisplayChanges(m_sceneGraph);

	//SQL
	int rc;
	char *sqlCommand;
	char *ErrMSG = 0;
	sqlite3_stmt *pResults;								//results of the query
	

	//OBJECTS IN THE WORLD Delete them all
	//prepare SQL Text
	sqlCommand = "DELETE FROM Objects";	 //will delete the whole object table.   Slightly risky but hey.
	rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand, -1, &pResults, 0);
	sqlite3_step(pResults);

	//Populate with our new objects
	std::wstring sqlCommand2;
	int numObjects = m_sceneGraph.size();	//Loop thru the scengraph.

	for (int i = 0; i < numObjects; i++)
	{
		std::stringstream command;
		command << "INSERT INTO Objects " 
			<<"VALUES(" << m_sceneGraph.at(i).ID << ","
			<< m_sceneGraph.at(i).chunk_ID  << ","
			<< "'" << m_sceneGraph.at(i).model_path <<"'" << ","
			<< "'" << m_sceneGraph.at(i).tex_diffuse_path << "'" << ","
			<< m_sceneGraph.at(i).posX << ","
			<< m_sceneGraph.at(i).posY << ","
			<< m_sceneGraph.at(i).posZ << ","
			<< m_sceneGraph.at(i).rotX << ","
			<< m_sceneGraph.at(i).rotY << ","
			<< m_sceneGraph.at(i).rotZ << ","
			<< m_sceneGraph.at(i).scaX << ","
			<< m_sceneGraph.at(i).scaY << ","
			<< m_sceneGraph.at(i).scaZ << ","
			<< m_sceneGraph.at(i).render << ","
			<< m_sceneGraph.at(i).collision << ","
			<< "'" << m_sceneGraph.at(i).collision_mesh << "'" << ","
			<< m_sceneGraph.at(i).collectable << ","
			<< m_sceneGraph.at(i).destructable << ","
			<< m_sceneGraph.at(i).health_amount << ","
			<< m_sceneGraph.at(i).editor_render << ","
			<< m_sceneGraph.at(i).editor_texture_vis << ","
			<< m_sceneGraph.at(i).editor_normals_vis << ","
			<< m_sceneGraph.at(i).editor_collision_vis << ","
			<< m_sceneGraph.at(i).editor_pivot_vis << ","
			<< m_sceneGraph.at(i).pivotX << ","
			<< m_sceneGraph.at(i).pivotY << ","
			<< m_sceneGraph.at(i).pivotZ << ","
			<< m_sceneGraph.at(i).snapToGround << ","
			<< m_sceneGraph.at(i).AINode << ","
			<< "'" << m_sceneGraph.at(i).audio_path << "'" << ","
			<< m_sceneGraph.at(i).volume << ","
			<< m_sceneGraph.at(i).pitch << ","
			<< m_sceneGraph.at(i).pan << ","
			<< m_sceneGraph.at(i).one_shot << ","
			<< m_sceneGraph.at(i).play_on_init << ","
			<< m_sceneGraph.at(i).play_in_editor << ","
			<< m_sceneGraph.at(i).min_dist << ","
			<< m_sceneGraph.at(i).max_dist << ","
			<< m_sceneGraph.at(i).camera << ","
			<< m_sceneGraph.at(i).path_node << ","
			<< m_sceneGraph.at(i).path_node_start << ","
			<< m_sceneGraph.at(i).path_node_end << ","
			<< m_sceneGraph.at(i).parent_id << ","
			<< m_sceneGraph.at(i).editor_wireframe << ","
			<< "'" << m_sceneGraph.at(i).name << "'" << ","

			<< m_sceneGraph.at(i).light_type << ","
			<< m_sceneGraph.at(i).light_diffuse_r << ","
			<< m_sceneGraph.at(i).light_diffuse_g << ","
			<< m_sceneGraph.at(i).light_diffuse_b << ","
			<< m_sceneGraph.at(i).light_specular_r << ","
			<< m_sceneGraph.at(i).light_specular_g << ","
			<< m_sceneGraph.at(i).light_specular_b << ","
			<< m_sceneGraph.at(i).light_spot_cutoff << ","
			<< m_sceneGraph.at(i).light_constant << ","
			<< m_sceneGraph.at(i).light_linear << ","
			<< m_sceneGraph.at(i).light_quadratic

			<< ")";
		std::string sqlCommand2 = command.str();
		rc = sqlite3_prepare_v2(m_databaseConnection, sqlCommand2.c_str(), -1, &pResults, 0);
		sqlite3_step(pResults);	
	}
	MessageBox(NULL, L"Objects Saved", L"Notification", MB_OK);
}

void ToolMain::onActionSaveTerrain()
{
	m_d3dRenderer.SaveDisplayChunk(&m_chunk);
}

void ToolMain::Tick(MSG *msg)
{
	//The actual update, called whenever no new Windows message is received

	//Object selection actions processing
	HandleInputSelectObject();

	//Editor Mode actions processing
	HandleInputEditorMode();

	//Camera Focus action processing
	HandleInputCameraFocus();

	//Determine which (if multiple) edit mode data struct to give
	EditModeData* data;
	switch(m_editorMode)
	{
		case EditorMode::Edit:
			data = &m_editModeData;
			break;

		default:
			data = nullptr;
			break;
	}

	//Renderer Update Call
	m_d3dRenderer.Tick(&m_toolInputCommands, data);

	// End of tick, zero the mouse deltas
	m_toolInputCommands.m_mouseDelta[0] = 0;
	m_toolInputCommands.m_mouseDelta[1] = 0;
}

void ToolMain::UpdateInput(MSG* msg)
{
	// The update called when new input windows message is received
	switch (msg->message)
	{
		//Global inputs,  mouse position and keys etc
		case WM_KEYDOWN:
			m_keyArray[msg->wParam] = true;
			break;

		case WM_KEYUP:
			m_keyArray[msg->wParam] = false;
			break;

		case WM_MOUSEMOVE:
		{
			const int newMouseX = GET_X_LPARAM(msg->lParam);
			const int newMouseY = GET_Y_LPARAM(msg->lParam);
			m_toolInputCommands.m_mouseDelta[0] = newMouseX - m_toolInputCommands.m_mousePos[0];
			m_toolInputCommands.m_mouseDelta[1] = newMouseY - m_toolInputCommands.m_mousePos[1];
			m_toolInputCommands.m_mousePos[0] = newMouseX;
			m_toolInputCommands.m_mousePos[1] = newMouseY;
			break;
		}

		case WM_MOUSEWHEEL:
			if (GET_WHEEL_DELTA_WPARAM(msg->wParam) > 0)
			{
				m_mouseArray[(int)MouseInput::WheelRollUp] = true;
				m_mouseArray[(int)MouseInput::WheelRollDown] = false;
			}
			else if (GET_WHEEL_DELTA_WPARAM(msg->wParam) < 0)
			{
				m_mouseArray[(int)MouseInput::WheelRollDown] = true;
				m_mouseArray[(int)MouseInput::WheelRollUp] = false;
			}
			else
			{
				m_mouseArray[(int)MouseInput::WheelRollUp] = false;
				m_mouseArray[(int)MouseInput::WheelRollDown] = false;
			}

			break;

		case WM_LBUTTONDOWN:	//mouse button down,  you will probably need to check when its up too
			m_mouseArray[(int)MouseInput::LButtonDown] = true;
			break;

		case WM_LBUTTONUP:
			m_mouseArray[(int)MouseInput::LButtonDown] = false;
			break;

		case WM_RBUTTONDOWN:	//mouse button down,  you will probably need to check when its up too
			m_mouseArray[(int)MouseInput::RButtonDown] = true;
			break;

		case WM_RBUTTONUP:
			m_mouseArray[(int)MouseInput::RButtonDown] = false;
			break;

		case WM_MBUTTONDOWN:
			m_mouseArray[(int)MouseInput::WheelButtonDown] = true;
			break;

		case WM_MBUTTONUP:
			m_mouseArray[(int)MouseInput::WheelButtonDown] = false;
			break;
	}

	// parse raw input into the action buffer based on mapping
	for(int i = 0; i < (int)Actions::MaxNum; i++)
	{
		const auto mapping = m_inputMapping.GetMapping((Actions)i);
		// if mapping available
		if (mapping.charId >= 0) 
		{
			// determine whether the action needs to be activated or not
			bool inputActive = mapping.isMouse ? m_mouseArray[mapping.charId] : m_keyArray[mapping.charId]; 
			if (inputActive) 
				m_toolInputCommands.SetState((Actions)i);
			else
				m_toolInputCommands.SetState((Actions)i, false);
		}
	}

	//Mouse scroll reset
	m_mouseArray[(int)MouseInput::WheelRollUp] = false;
	m_mouseArray[(int)MouseInput::WheelRollDown] = false;
}

void ToolMain::HandleInputCameraFocus()
{
	if (!m_toolInputCommands.GetState(Actions::ArcCameraModeToggle))
		return;

	if (!m_d3dRenderer.GetCamera()->HasFocus()
		&& !m_selectedObjects.empty())
	{
		
		m_d3dRenderer.SetCameraFocus(m_selectedObjects[0]);
		return;
	}

	m_d3dRenderer.SetCameraFocus();
}

void ToolMain::HandleInputSelectObject()
{
	if (m_toolInputCommands.GetState(Actions::SelectObject))
	{
		auto testResult = m_d3dRenderer.PerformRayTest(
			m_toolInputCommands.m_mousePos[0],
			m_toolInputCommands.m_mousePos[1]);

		// Store only Id as the DisplayList memory allocation addresses etc. get changed
		// This makes the underlying object pointer unreliable
		if (testResult.Id >= 0
			&& std::find(m_selectedObjects.begin(), m_selectedObjects.end(), testResult.Id) == m_selectedObjects.end())
		{
			m_selectedObjects.push_back(testResult.Id);
			testResult.IntersectedObject->MarkSelected();
		}
	}
	else if (m_toolInputCommands.GetState(Actions::DeselectObject))
	{
		auto testResult = m_d3dRenderer.PerformRayTest(
			m_toolInputCommands.m_mousePos[0],
			m_toolInputCommands.m_mousePos[1]);

		if (testResult.Id >= 0)
		{
			auto item = std::find(m_selectedObjects.begin(), m_selectedObjects.end(), testResult.Id);
			if (item != m_selectedObjects.end())
			{
				m_selectedObjects.erase(item);
				testResult.IntersectedObject->UnmarkSelected();
			}
		}
	}
}

void ToolMain::HandleInputEditorMode()
{
	if (m_toolInputCommands.GetState(Actions::ToggleEditMode))
	{
		m_editorMode = m_editorMode == EditorMode::Default ? EditorMode::Edit : EditorMode::Default;
		m_editModeData.Reset();
	}

	//-------LOCKING AXES--------
	if (m_toolInputCommands.GetState(Actions::ToggleEditingAxisX))
		m_editModeData.ToggleEditAxis(EditModeData::Axis::X);

	if (m_toolInputCommands.GetState(Actions::ToggleEditingAxisY))
		m_editModeData.ToggleEditAxis(EditModeData::Axis::Y);

	if (m_toolInputCommands.GetState(Actions::ToggleEditingAxisZ))
		m_editModeData.ToggleEditAxis(EditModeData::Axis::Z);

	// If editor mode is not on and there are no selected objects...
	//... don't bother with the rest of input handling
	if (m_editorMode == EditorMode::Default
		|| m_selectedObjects.size() < 1)
	{
		return;
	}

	// Container for DisplayObjects, due to seemingly asynchronous nature this is valid only...
	// during this call (hopefully)
	std::vector<DisplayObject*> selectedObjects;

	//--------SCALING OBJECT--------
	if (m_toolInputCommands.GetState(Actions::ObjectSizeUp, false))
	{
		if (selectedObjects.size() < 1)
			selectedObjects = m_d3dRenderer.GetSelectedDisplayObjects(m_selectedObjects);

		for (auto it : selectedObjects)
		{
			it->Scale(Vector3(
				m_editModeData.m_scalingStep * (int)m_editModeData.IsAxisUnlocked(EditModeData::Axis::X),
				m_editModeData.m_scalingStep * (int)m_editModeData.IsAxisUnlocked(EditModeData::Axis::Y),
				m_editModeData.m_scalingStep * (int)m_editModeData.IsAxisUnlocked(EditModeData::Axis::Z)
			));
		}
	}

	if (m_toolInputCommands.GetState(Actions::ObjectSizeDown, false))
	{
		if (selectedObjects.size() < 1)
			selectedObjects = m_d3dRenderer.GetSelectedDisplayObjects(m_selectedObjects);

		for (auto it : selectedObjects)
		{
			it->Scale(Vector3(
				-m_editModeData.m_scalingStep * (int)m_editModeData.IsAxisUnlocked(EditModeData::Axis::X),
				-m_editModeData.m_scalingStep * (int)m_editModeData.IsAxisUnlocked(EditModeData::Axis::Y),
				-m_editModeData.m_scalingStep * (int)m_editModeData.IsAxisUnlocked(EditModeData::Axis::Z)
			));
		}
	}

	//--------ROTATING OBJECT--------
	if (m_toolInputCommands.GetState(Actions::ToggleObjectRotate))
		m_editModeData.m_rotating = !m_editModeData.m_rotating;

	if (m_editModeData.m_rotating)
	{
		const float mouseFactor = m_toolInputCommands.m_mouseDelta[0] * 0.01;
		if (mouseFactor != 0)
		{
			if (selectedObjects.size() < 1)
				selectedObjects = m_d3dRenderer.GetSelectedDisplayObjects(m_selectedObjects);

			for (auto it : selectedObjects)
			{
				it->Rotate(Vector3(
					m_editModeData.m_rotateStep * mouseFactor * (int)m_editModeData.IsAxisUnlocked(EditModeData::Axis::X),
					m_editModeData.m_rotateStep * mouseFactor * (int)m_editModeData.IsAxisUnlocked(EditModeData::Axis::Y),
					m_editModeData.m_rotateStep * mouseFactor * (int)m_editModeData.IsAxisUnlocked(EditModeData::Axis::Z)
				));
			}
		}
	}

	//--------MOVING OBJECT--------
	if (m_toolInputCommands.GetState(Actions::ToggleObjectMoveByMouse))
		m_editModeData.m_mouseMoving = !m_editModeData.m_mouseMoving;

	if (m_editModeData.m_mouseMoving 
		&& m_toolInputCommands.GetState(Actions::ObjectMoveToMouse, false))
	{
		auto result = m_d3dRenderer.PerformRayTest(
			m_toolInputCommands.m_mousePos[0],
			m_toolInputCommands.m_mousePos[1]);

		//TODO: Check for terrain and move the object to result.IntersectionPoint
	}
}
