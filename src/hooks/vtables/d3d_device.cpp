#include "../hooks.h"
#include "../../menu/menu.h"

long __stdcall hooks::d3d_device::present::fn(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region) {
	static const auto original = m_d3d_device->get_original<T>(index);

	IDirect3DVertexDeclaration9* vert_dec = nullptr;
	IDirect3DVertexShader9* vert_shader = nullptr;

	device->GetVertexDeclaration(&vert_dec);
	device->GetVertexShader(&vert_shader);

	if (GetAsyncKeyState(globals::config::menuKey.Key) & 1)
		globals::config::menuKey.Toggled = !globals::config::menuKey.Toggled;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	player_esp->CallEsp();
	menu->on_paint();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	device->SetVertexShader(vert_shader);
	device->SetVertexDeclaration(vert_dec);

	return original(device, src_rect, dest_rect, dest_wnd_override, dirty_region);
}

long __stdcall hooks::d3d_device::reset::fn(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_params) {
	static const auto original = m_d3d_device->get_original<T>(index);

	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto ret = original(device, present_params);
	ImGui_ImplDX9_CreateDeviceObjects();

	return ret;
}