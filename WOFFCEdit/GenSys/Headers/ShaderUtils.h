#pragma once
#include "ShaderManager.h"

#include "DirectXMath.h"

//! enum used as a way to distinguish to which shader stage to send a buffer to
enum PipelineStage : int
{
	Vertex = 0,
	Pixel,
	Geo,
	Domain,
	Hull,
	Compute
};

// FUNCTIONS //

//SETUP BUFFER FUNCTION-----------------------------------------------------------------------
//! uses a set of predefined params used most commonly by the program to properly setup a buffer
//! uses default values so the different param combos are supported if needed
//! BufferType determines the struct to use for the size and length
template<typename BufferType>
void SetupBuffer(
	ID3D11Device* directDevice,
	ID3D11Buffer** buffer,
	D3D11_USAGE usage = D3D11_USAGE_DYNAMIC,
	int bindFlags = D3D11_BIND_CONSTANT_BUFFER,
	int accessFlags = D3D11_CPU_ACCESS_WRITE,
	int miscFlags = 0,
	int stride = 0,
	int count = 1)
{
	D3D11_BUFFER_DESC outBufferDesc;
	ZeroMemory(&outBufferDesc, sizeof(outBufferDesc));
	outBufferDesc.Usage = usage;
	outBufferDesc.ByteWidth = sizeof(BufferType) * count;
	outBufferDesc.BindFlags = bindFlags;
	outBufferDesc.CPUAccessFlags = accessFlags;
	outBufferDesc.MiscFlags = miscFlags;
	outBufferDesc.StructureByteStride = stride;

	directDevice->CreateBuffer(&outBufferDesc, nullptr, buffer);
}

//MAP BUFFER TO POINTER FUNCTIONS------------------------------------------------------------------------
//! uses a set of predefined params to allow access to a buffer. Uses default definitions hence can be modified if needed
//! since the T type pointer is returned, T type needs to match the original setup type used, this is up to the developer to uphold
template<typename T>
T* MapBufferToPointer(ID3D11DeviceContext* context, ID3D11Buffer* buffer, D3D11_MAP mapping = D3D11_MAP_WRITE_DISCARD, int subID = 0)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	result = context->Map(buffer, subID, mapping, 0, &mappedResource);
	return static_cast<T*>(mappedResource.pData);
}

//SETUP SAMPLER FUNCTION -----------------------------------------------------------------------
//! uses a set of predefined params used most commonly by the program to properly setup a sampler
//! uses default values so the different param combos are supported if needed
void SetupSampler(
	ID3D11Device* device,
	ID3D11SamplerState* sampleState,
	D3D11_TEXTURE_ADDRESS_MODE modeU = D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_MODE modeV = D3D11_TEXTURE_ADDRESS_CLAMP,
	D3D11_TEXTURE_ADDRESS_MODE modeW = D3D11_TEXTURE_ADDRESS_CLAMP,
	float borderColor_R = 0.f,
	float borderColor_G = 0.f,
	float borderColor_B = 0.f,
	float borderColor_A = 0.f,
	D3D11_FILTER filter = D3D11_FILTER_ANISOTROPIC,
	float MipLoadBias = 0.f,
	int MaxAnisotropy = 1,
	D3D11_COMPARISON_FUNC cmpFunc = D3D11_COMPARISON_ALWAYS,
	float MinLOD = 0,
	float MaxLOD = D3D11_FLOAT32_MAX);

//FINALIZE BUFFER FUNCTION --------------------------------------------------------------------
//! unmaps the buffer and send it to the correct register in the correct stage
void FinalizeBuffer(ID3D11DeviceContext* context, ID3D11Buffer* buffer, PipelineStage stage, int bufferID = 0);

//RELEASE FUNCTIONS -------------------------------------------------------------------------
//! safely relase the memory dedicated to the respective DX11 objects
void ReleaseBuffer(ID3D11Buffer** buffer);
void ReleaseSampler(ID3D11SamplerState** sampler);

void SetupRWTextureBuffer(ID3D11Device* directDevice, Texture* texture);
void SetupStandardBufferUAV(ID3D11Device* directDevice, ID3D11Buffer* buffer, ID3D11UnorderedAccessView** UAVBuffer);