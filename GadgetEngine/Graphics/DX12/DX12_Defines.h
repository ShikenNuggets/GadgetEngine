#ifndef GADGET_DX12_DEFINES_H
#define GADGET_DX12_DEFINES_H

#include <dxgi1_6.h>
#include <d3d12.h>

//--------------------------------------------------------------------
//Undefine some "standard" macros that tend to cause problems
#undef min
#undef max
//--------------------------------------------------------------------

//These will make it easier to increase interface versions later
using ID3D12_Debug					= ID3D12Debug3;
using ID3D12_DebugDevice			= ID3D12DebugDevice2;
using ID3D12_Device					= ID3D12Device8;
using ID3D12_GraphicsCommandList	= ID3D12GraphicsCommandList6;

using IDXGI_Adapter					= IDXGIAdapter4;
using IDXGI_Factory					= IDXGIFactory7;

#endif //!GADGET_DX12_DEFINES_H