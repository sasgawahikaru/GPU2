#include "Light.h"
using namespace DirectX;

ID3D12Device* Light::device = nullptr;
void Light::StaticInitialize(ID3D12Device* device)
{
	assert(!Light::device);
	assert(device);
	Light::device = device;
}
void Light::Initialize()
{
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;
	result = device->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	TransferConstBuffer();
}
void Light::SetLightDir(const XMVECTOR& lightdir)
{
	this->lightdir = XMVector3Normalize(lightdir);
	dirty = true;
}
void Light::SetLightColor(const XMFLOAT3& lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}
void Light::Update()
{
	if (dirty) {
		TransferConstBuffer();
		dirty = false;
	}
}
void Light::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}

Light* Light::Create()
{
	Light* instance = new Light();

	instance->Initialize();

	return instance;
}
void Light::TransferConstBuffer()
{
	HRESULT result;
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightv = lightdir;
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}
}