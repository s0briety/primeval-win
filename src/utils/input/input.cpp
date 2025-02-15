#include "../../sdk/interfaces.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace input {
	void init() {
		auto creation_parameters = D3DDEVICE_CREATION_PARAMETERS();

		interfaces::m_d3d_device->GetCreationParameters(&creation_parameters);

		m_hwnd = creation_parameters.hFocusWindow;

		m_original_wnd_proc = reinterpret_cast<WNDPROC>(SetWindowLongA(m_hwnd, GWL_WNDPROC, reinterpret_cast<long>(wnd_proc)));
	}

	void undo() { SetWindowLongPtrA(m_hwnd, GWL_WNDPROC, reinterpret_cast<long>(m_original_wnd_proc)); }

	std::string GetKeyNameFromVK(int vkCode)
	{
		UINT scanCode = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
		char keyName[128];

		if (GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName)) > 0)
		{
			return std::string(keyName);
		}
		return "Unbound";
	}

	bool get_key(KeyBind& bind) {
		e_key_mode mode = static_cast<e_key_mode>(bind.Mode);

		if (mode != ALWAYS_ON && bind.Key == 0)
			return bind.Toggled = false;

		bool result = false;

		switch (mode) {
		case ALWAYS_OFF:
			result = false;
			break;
		case ON_KEY:
			result = GetKeyState(bind.Key) & 0x8000;
			break;
		case TOGGLE:
			result = GetKeyState(bind.Key) & 1;
			break;
		case AWAY_KEY:
			result = !(GetKeyState(bind.Key) & 0x8000);
			break;
		case ALWAYS_ON:
			result = true;
			break;
		}

		if (bind.Toggled != result)
			bind.Toggled = result;
		return result;
	}

	long __stdcall wnd_proc(HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param) {
		if (m_blocked) { 
			ImGui_ImplWin32_WndProcHandler(hwnd, msg, w_param, l_param);

			if (!m_Inactive)
			{
				m_Inactive = true;
				CallWindowProc(m_original_wnd_proc, hwnd, WM_ACTIVATE, WA_INACTIVE, 0);
			}
			else if (m_Inactive)
			{
				m_Inactive = false;
				CallWindowProc(m_original_wnd_proc, hwnd, WM_ACTIVATE, WA_INACTIVE, 0);
			}
		}

		return m_blocked ? m_blocked : CallWindowProc(m_original_wnd_proc, hwnd, msg, w_param, l_param);
	}

	bool m_blocked = false;
	bool m_Inactive = false;

	HWND m_hwnd;
	WNDPROC m_original_wnd_proc;
}