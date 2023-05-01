#pragma once
#include "TextureManager.h"

constexpr int MaxRiverGraphNodes = 1000;

class ShaderManager
{
	ShaderManager();
	~ShaderManager();

public:

	static ShaderManager* Get() { static ShaderManager instance; return &instance; };

	void SendDataToDevice();
	void UpdateBasicBuffer() const;
	void Compute(const GenerationSteps step, const int x, const int y, const int z) const;
	void LoadUserTexture(const UserTexturesNames textureName, const std::wstring& filePath);
	void LoadUserParams(FullTerrainContext& fullTerrainParams);
	void ExportGeneratedTexture(const std::wstring& filePath, const GeneratedTexturesNames name) const;
	void SetupGeneratedTexture(const GeneratedTexturesNames name, const int width, const int height);
	void LoadComputeShader(const std::wstring& filename, const GenerationSteps step);
	bool HasTexture(const GeneratedTexturesNames name) const;
	void CopyRiverNodesFromGPU();
	void GetRiverGraph(RiverGraph* riverGraph) const { memcpy(riverGraph,&_riverGraph, sizeof(riverGraph)); };

protected:

	ID3D11Device* _directDevice = nullptr;                     
	ID3D11DeviceContext* _directDeviceContext = nullptr;       
	ID3D11Buffer* _userParametersBuffer;
	ID3D11Buffer* _basicBuffer;
	ID3D11Buffer* _riverNodeBuffer;
	ID3D11UnorderedAccessView* _riverGraphUAV;
	std::map<GenerationSteps, ID3D11ComputeShader*> _computeShaderMap;
	ID3D11SamplerState* _sampleState;
	CombinedTerrainContext _userParams = CombinedTerrainContext();
	RiverGraph _riverGraph;

	Texture* CreateRWTexture(const int width, const int height, const DXGI_FORMAT format = DXGI_FORMAT_R32_FLOAT);
	Texture* LoadTextureFromFile(const std::wstring& filePath, const bool isInput = false);

	TextureManager textureManager;

	// function prototypes
	void InitD3D();
	// sets up and initializes Direct3D
	void CleanD3D();         // closes Direct3D and releases memory
};