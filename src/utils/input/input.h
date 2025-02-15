#pragma once

enum e_key_mode {
	ALWAYS_OFF,
	ON_KEY,
	TOGGLE,
	AWAY_KEY,
	ALWAYS_ON
	// 0 = Always Off, 1 = Hold, 2 = Toggle, 3 = Key Release, 4 = Always On
};

namespace input {
	void init();

	void undo();

	long __stdcall wnd_proc(HWND hwnd, uint32_t msg, uint32_t w_param, uint32_t l_param);

	std::string GetKeyNameFromVK(int vkCode);

	extern bool get_key(KeyBind& bind);

	extern bool m_blocked;
	extern bool m_Inactive;

	extern HWND m_hwnd;
	extern WNDPROC m_original_wnd_proc;

};