#pragma once

// directx stuff
#include <d2d1.h>
#include <d2d1_2.h>
#include <dwrite.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <dxgi1_4.h>

// imgui & kiero
#include "../../../libs/imgui/imgui.h"
#include "../../../libs/imgui/imgui_impl_win32.h"
#include "../../../libs/imgui/imgui_impl_dx12.h"

#include "../../../libs/kiero/kiero.h"
#include "../../../libs/imgui/imgui_impl_dx11.h"

#include "../../../Assets/Font/ProductSans.h"

// rest of shit
#include "DeviceType.h"

// Dx11
namespace DxComps {
	// hook storage
    void* __o__Present;
    void* __o__Resize;

	// init stuff
	bool InitCtx;

	// mc window
	HWND Window;
	Vector2<float> Size;

	// devices
	ID3D11Device* D3D11Device = nullptr;
	ID3D12Device5* D3D12Device = nullptr;

    // other bits and bobs
    ID3D11DeviceContext* Context = nullptr;
    ID3D11RenderTargetView* RenderView = nullptr;
}

void MenuRender() {
	// gonnae render basic test stuff here in a second
	ImDrawList* list = ImGui::GetBackgroundDrawList();

    ImFont* font = ImGui::GetIO().Fonts->Fonts[0]; // get main font

    // some random bits and bobs to show it working
	list->AddRectFilled(ImVec2(10, 10), ImVec2(90, 30), ImColor(255, 255, 255), 5, NULL);
    list->AddText(font, 16, ImVec2(12, 12), ImColor(0, 0, 0), "Hello, World!");
}

void DirectUpdate(IDXGISwapChain* swapchain) {
    if (DxComps::D3D11Device == nullptr)
        return; // no device, no update (race condition of resize being called before present)

    DxComps::D3D11Device->GetImmediateContext(&DxComps::Context);

    // temp backbuffer
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    DxComps::D3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &DxComps::RenderView);

    pBackBuffer->Release();
    
    // init imgui with the new things
    ImGui_ImplWin32_Init(DxComps::Window);
    ImGui_ImplDX11_Init(DxComps::D3D11Device, DxComps::Context);
}

HRESULT PresentDetour(IDXGISwapChain* swapchain, UINT syncIntveral, UINT flags)
{
	DxComps::Window = FindWindowA(nullptr, "Minecraft"); // get mc by title

	RECT rect;
	if (GetWindowRect(DxComps::Window, &rect)) {
		DxComps::Size = Vector2<float>(
			rect.right - rect.left, // width
			rect.bottom - rect.top  // height
		);
	}

	DeviceType type = DeviceType::UNKNOWN;

	if (!DxComps::Window) {
        std::cout << "[D3D11] Failed to find Minecraft window" << std::endl;
        kiero::shutdown();

		return CallFunc<HRESULT, IDXGISwapChain*, UINT, UINT>(
			DxComps::__o__Present,
			swapchain,
			syncIntveral,
			flags
		);
	}

	if (SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&DxComps::D3D11Device))))
		type = DeviceType::D3D11;

	if (SUCCEEDED(swapchain->GetDevice(IID_PPV_ARGS(&DxComps::D3D12Device))))
		type = DeviceType::D3D12;

	if (type == DeviceType::UNKNOWN) {
		// ???????????
		return CallFunc<HRESULT, IDXGISwapChain*, UINT, UINT>(
			DxComps::__o__Present,
			swapchain,
			syncIntveral,
			flags
		);
	}

	if (type == DeviceType::D3D11)
    {
		if (!DxComps::InitCtx)
        {
            // init imgui & load a compressed font for us to use
			ImGui::CreateContext();
            ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(ProductSans::compressed_data, ProductSans::compressed_size, 48.f);
			
			DirectUpdate(swapchain);

            DxComps::InitCtx = true; // so we dont repeat this
		}

        // ready imgui for a new frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// render frame
		MenuRender();

		// finish frame
		ImGui::EndFrame();
		ImGui::Render();

        // render comps to backbuffer
		DxComps::Context->OMSetRenderTargets(1, &DxComps::RenderView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // reset backbuffer
		//DxComps::Context->OMSetRenderTargets(0, 0, 0);
	}

	if (type == DeviceType::D3D12) {
		// uninstall dx12 from the game
        // NOTE: this cannot be undone without relinking the internal minecraft pointers
        // to a new dx12 device, which would require allocating it in our dll
        // meaning once ejected it'll unload that new device causing a crash
        // theres ways around it but they're not worth the effort
		static_cast<ID3D12Device5*>(DxComps::D3D12Device)->RemoveDevice();
	}

	return CallFunc<HRESULT, IDXGISwapChain*, UINT, UINT>(
		DxComps::__o__Present,
		swapchain,
		syncIntveral,
		flags
	);
}

HRESULT ResizeDetour(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
    if (DxComps::RenderView) {
        DxComps::Context->OMSetRenderTargets(0, 0, 0);
        DxComps::RenderView->Release();
    }

    // have to call the resize event before we update our directx stuff
    HRESULT hr = CallFunc<HRESULT, IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT>(
        DxComps::__o__Resize,
        pSwapChain,
        BufferCount,
        Width, Height,
        NewFormat, SwapChainFlags
    );

    DirectUpdate(pSwapChain);

    return hr;
}

class DirectXHook : public FuncHook {
public:
	bool Initialize() override {
        auto dx12 = kiero::init(kiero::RenderType::D3D12);
		if (dx12 != kiero::Status::Success && kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success)
		{
			std::cout << "[D3D11] Failed to create hook" << std::endl;
			return false;
		}
        
        // present so we can render our frame (NEEDED)
        kiero::bind(dx12 == kiero::Status::Success ? 140 : 8, (void**)&DxComps::__o__Present, PresentDetour);

        // resize so we can handle our resources more effectively (not needed but i would recommend keeping this)
        kiero::bind(dx12 == kiero::Status::Success ? 145 : 13, (void**)&DxComps::__o__Resize, ResizeDetour);

		std::cout << "[D3D11] Successfully enabled hook" << std::endl;

		return true;
	}

	static DirectXHook& Get() {
		static DirectXHook instance;
		return instance;
	}
};