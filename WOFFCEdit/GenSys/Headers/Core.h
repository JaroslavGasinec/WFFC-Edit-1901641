#pragma once
#include "CoreDataTypes.h"
#include <string>

#ifdef GENSYSCORE_EXPORTS
#define GENSYSCORE_API __declspec(dllexport)
#else
#define GENSYSCORE_API __declspec(dllimport)
#endif

static constexpr int BaseResolution = 512;
static const std::wstring S_Noise = L"CS_Noise.cso";
static const std::wstring S_TerrainA = L"CS_TerrainA.cso";
static const std::wstring S_River = L"CS_River.cso";
static const std::wstring S_TerrainB = L"CS_TerrainB.cso";
static const std::wstring S_Layers = L"CS_Layers.cso";
static const std::wstring S_Foliage = L"CS_Foliage.cso";

namespace Gensys
{

namespace CoreSteps
{

//! @brief Function to generate the value noise texture
extern "C++" GENSYSCORE_API void PrepareGenerateValueNoise(const std::wstring& shader = S_Noise);

//! @brief Function to generate the value noise texture
extern "C++" GENSYSCORE_API void GenerateValueNoise(const ValueNoiseContext* data = nullptr);

//! @brief Function to generate the first terrain height map
extern "C++" GENSYSCORE_API void PrepareGeneratePhase1Terrain(const std::wstring& shader = S_TerrainA);

//! @brief Function to generate the first terrain height map
extern "C++" GENSYSCORE_API void GeneratePhase1Terrain(const bool blockSending = false);

//! @brief Function to generate the first terrain height map
extern "C++" GENSYSCORE_API void PrepareGenerateRiverMap(const std::wstring& shader = S_River);

//! @brief Function to generate the river map
extern "C++" GENSYSCORE_API void GenerateRiverMap(const RiverMapContext* data = nullptr);

//! @brief Function to generate the modified terrain height map
extern "C++" GENSYSCORE_API void PrepareGeneratePhase2Terrain(const std::wstring& shader = S_TerrainB, const bool forceNewTerrain = false);

//! @brief Function to generate the modified terrain height map
extern "C++" GENSYSCORE_API void GeneratePhase2Terrain(const TerrainPhase2Context* data = nullptr);

//! @brief Function to generate terrain layer Map 
extern "C++" GENSYSCORE_API void PrepareGenerateTerrainLayerMap(const std::wstring& shader = S_Layers);

//! @brief Function to generate terrain layer Map 
extern "C++" GENSYSCORE_API void GenerateTerrainLayerMap(const LayerMapContext* data = nullptr);

//! @brief Function to generate foliage map fo the given terrain
extern "C++" GENSYSCORE_API void PrepareGenerateFoliageMap(const std::wstring& shader = S_Foliage);

//! @brief Function to generate foliage map fo the given terrain
extern "C++" GENSYSCORE_API void GenerateFoliageMap(const struct FoliageMapContext* data = nullptr);

}// CoreSteps namespace

//! @brief Function that intialises ShaderManager and the entire Gensys system
extern "C++" GENSYSCORE_API void Initialise();

//! @brief Function that exports a specific stored generated texture
//! Exports the last stored texture in GPU memory in the named slot
extern "C++" GENSYSCORE_API void ExportGeneratedTexture(const GeneratedTexturesNames name, const std::wstring& filepath);

//! @brief Function that exports a specific stored generated texture.
//! Exports the last stored texture in GPU memory in the named slot.
extern "C++" GENSYSCORE_API void ExportGeneratedRiver(RiverGraph* riverOut);

//! @brief Function that loads and send the full user context to the GPU.
//! If paths to user textures are supplied, those will be also loaded into GPU.
//! Should be rarely called by itself as GenerateFullTerrain does this step already.
extern "C++" GENSYSCORE_API void SendFullContextToDevice(FullTerrainContext& context);

//! @brief Function to aggregate the full terrain generation process.
extern "C++" GENSYSCORE_API void GenerateFullTerrain(FullTerrainContext* terrainContext = nullptr, RiverGraph* riverOut = nullptr);

}// Gensys namespace