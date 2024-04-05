#ifndef GADGET_DX12_DEFINES_H
#define GADGET_DX12_DEFINES_H

#include <dxgi1_6.h>
#include <d3d12.h>

//These will make it easier to increase interface versions later
using ID3D12_Device = ID3D12Device8;
using ID3D12_GraphicsCommandList = ID3D12GraphicsCommandList6;
using IDXGI_Factory = IDXGIFactory7;

#endif //!GADGET_DX12_DEFINES_H