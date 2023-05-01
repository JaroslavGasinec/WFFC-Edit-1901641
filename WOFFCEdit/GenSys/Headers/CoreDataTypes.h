#pragma once
#ifndef COREDTH
#define COREDTH

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "DirectXMath.h"

#pragma comment (lib, "d3d11.lib")

#ifdef _DEBUG
	#pragma comment (lib, "d3dx11d.lib")
	#pragma comment (lib, "d3dx10d.lib")
#else 
	#pragma comment (lib, "d3dx11.lib")
	#pragma comment (lib, "d3dx10.lib")
#endif

#include <map>
#include <string>
#include <vector>

enum class GenerationSteps : int
{
	Noise,
	TerrainPhaseA,
	Rivers,
	TerrainPhaseB,
	TerrainLayers,
	FoliageMap,
	Num
};

enum class UserTexturesNames : int
{
	UserTerrainOutline,
	UserTerrainFeatureMap,
	UserRiverOutline,
	Num
};

enum class GeneratedTexturesNames : int
{
	Noise,
	TerrainMap,
	RiverMap,
	RiverFlow,
	TerrainLayers,
	Foliage,
	Num
};

enum class RosgenRiverType : int
{
	Default
};

struct Cubic
{
	float a = 0;
	float b = 0;
	float c = 0;
	float d = 0;
};

struct RiverNode
{
	float UVPosX = -1;
	float UVPosY = -1;
	Cubic Previous;
	Cubic Next;
	RosgenRiverType Type = RosgenRiverType::Default;
	RiverNode* NextNode = nullptr;
	RiverNode* PreviousNode = nullptr;

	~RiverNode()
	{
		NextNode = nullptr;
		PreviousNode = nullptr;
	}
};

typedef std::map<int, RiverNode> RiverGraph;

struct UserParametersBuffer
{
	float ValueNoiseOctaves;
	float BlurPixelRadius;
	float Granularity;
	float TerrainOutlineGiven;
	float TerrainFeaturesGiven;
	float RiverGenerationIterations;
	float RiverResolution;
	float RiverThickness;
	float RiverAllowNodeMismatch;
	float RiversOnGivenFeatures;
	float RiversGiven;
	float RiverStrengthFactor;
	float NumberOfTerrainLayers;
	float NumberOfFoliageLayers;
	float FoliageWholeness;
	float MinUnitFoliageHeight;
};
static_assert(sizeof(UserParametersBuffer) % 16 == 0, "UserParametersBuffer is not correctly sized");

// This buffer is meant to be constant throughout each dispatch but should be changed frequently
struct BasicBuffer
{
	float RandomSeed;
	DirectX::XMFLOAT3 p2;
};
static_assert(sizeof(BasicBuffer) % 16 == 0, "BasicBuffer is not correctly sized");

struct RWComputeBuffer
{
	int ID = -1;
	int PreviousID = -1;
	int NextID = -1;
	float UVPosX = -1;
	float UVPosY = -1;
	float a1 = 0;
	float b1 = 0;
	float c1 = 0;
	float d1 = 0;
	float a2 = 0;
	float b2 = 0;
	float c2 = 0;
	float d2= 0;
	int Type = static_cast<int>(RosgenRiverType::Default);
};

struct Texture
{
	bool IsInput = false;
	std::wstring FilePath = L"";
	ID3D11ShaderResourceView* User_SRVDataView = nullptr;
	ID3D11Resource* User_D3DData = nullptr;
	ID3D11UnorderedAccessView* Gen_UAVDataView = nullptr;
	ID3D11Texture2D* Gen_GPUTexture = nullptr;

	~Texture()
	{
		if (User_SRVDataView)
			User_SRVDataView->Release();

		if (User_D3DData)
			User_D3DData->Release();

		if (Gen_UAVDataView)
			Gen_UAVDataView->Release();

		if (Gen_GPUTexture)
			Gen_GPUTexture->Release();
	}

	//! @brief Function that creates a CPU accessible buffer from the texture data on the GPU
	//! The buffer control is left to the callee, hence the cleanup is on them
	ID3D11Texture2D* AccessGenDataAsCPU(ID3D11DeviceContext* deviceContext, ID3D11Device* device) const
	{
		D3D11_TEXTURE2D_DESC desc;
		ID3D11Texture2D* bufferCopy;
		// Create texture for GPU Read and Write
		ZeroMemory(&desc, sizeof(desc));
		Gen_GPUTexture->GetDesc(&desc);

		// Create texture buffer for GPU -> CPU data flow
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		device->CreateTexture2D(&desc, NULL, &bufferCopy);

		deviceContext->CopyResource(bufferCopy, Gen_GPUTexture);

#ifdef _DEBUG
		constexpr int Width = 512;
		constexpr int Height = 512;
		
		std::vector<FLOAT> Read;
		
		for (int i = 0; i < Width; ++i)
			for (int j = 0; j < Height; ++j)
				Read.push_back(-1.0);
		
		D3D11_MAPPED_SUBRESOURCE ResourceDesc = {};
		deviceContext->Map(bufferCopy, 0, D3D11_MAP_READ, 0, &ResourceDesc);
		
		if (ResourceDesc.pData)
		{
			int const BytesPerPixel = sizeof(FLOAT);
			for (int i = 0; i < Height; ++i)
			{
				std::memcpy(
					(byte*)Read.data() + Width * BytesPerPixel * i,
					(byte*)ResourceDesc.pData + ResourceDesc.RowPitch * i,
					Width * BytesPerPixel);
			}
		}

		deviceContext->Unmap(bufferCopy, 0);
#endif

		return bufferCopy;
	}
};

struct ValueNoiseContext
{
	// GenerateValueNoise()
	int ValueNoiseOctaves = 0;
	double BlurPixelRadius = 0;
	double Granularity = 0;
};

struct TerrainPhase1Context
{
	// GeneratePhase2Terrain()
	bool TerrainOutlineGiven = false;
	bool TerrainFeaturesGiven = false;
};

struct RiverMapContext
{
	// GenerateRiverMap()
	int RiverGenerationIterations = 0;
	float RiverResolution = 0;
	int RiverThickness = 0;
	bool RiverAllowNodeMismatch = false;
	bool RiversOnGivenFeatures = false;
	bool RiversGiven = false;
};

struct TerrainPhase2Context
{
	// GeneratePhase2Terrain()
	float RiverStrengthFactor = 0;
};

struct LayerMapContext
{
	// GenerateTerrainLayerMap()
	int NumberOfTerrainLayers = 0;
};

struct FoliageMapContext
{
	// GenerateFoliageMap()
	int NumberOfFoliageLayers = 0;
	float FoliageWholeness = 1.f;
	float MinUnitFoliageHeight = 0.f;
};

struct CombinedTerrainContext :
	public ValueNoiseContext,
	public TerrainPhase1Context,
	public RiverMapContext,
	public TerrainPhase2Context,
	public LayerMapContext,
	public FoliageMapContext
{
	CombinedTerrainContext() = default;
};

struct FullTerrainContext : public CombinedTerrainContext
{
	// GeneratePhase1Terrain()
	std::wstring User_TerrainOutlineMap;
	std::wstring User_TerrainFeatureMap;

	// GenerateRiverMap()
	std::wstring User_RiverOutline;
};

#endif