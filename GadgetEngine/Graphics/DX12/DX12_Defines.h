#ifndef GADGET_DX12_DEFINES_H
#define GADGET_DX12_DEFINES_H

#include <dxgi1_6.h>
#include <d3d12.h>

//--------------------------------------------------------------------
//Undefine some "standard" macros that tend to cause problems
#undef min
#undef max
//--------------------------------------------------------------------

//Using these will make it easier to update interface versions later
//Avoid increasing these if it's not absolutely necessary
//Higher version = more features, lower version = better compatibility
using ID3D12_Debug					= ID3D12Debug1;
using ID3D12_DebugDevice			= ID3D12DebugDevice1;
using ID3D12_Device					= ID3D12Device5;
using ID3D12_Fence					= ID3D12Fence1;
using ID3D12_GraphicsCommandList	= ID3D12GraphicsCommandList4;
using ID3D12_InfoQueue				= ID3D12InfoQueue1;
using ID3D12_Resource				= ID3D12Resource;

using IDXGI_Adapter					= IDXGIAdapter1;
using IDXGI_Factory					= IDXGIFactory6;
using IDXGI_SwapChain				= IDXGISwapChain3;

#endif //!GADGET_DX12_DEFINES_H