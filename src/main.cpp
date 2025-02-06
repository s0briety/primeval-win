#include "common_includes.h"
#include "hooks/hooks.h"
#include "events/events.h"
#include "globals.h"

inline std::vector<KeyBind*> keyBinds = {
	&globals::config::QuickPeekKey,
	&globals::config::forceBaimKey,
	&globals::config::DoubleTapKey,
	&globals::config::HideshotsKey,
	&globals::config::IdealTickKey,
	&globals::config::HitchanceOverrideKey,
	&globals::config::MinDamageOverrideKey,
	&globals::config::ThirdPersonKey
};

inline bool checkKeybinds() {
	for (auto bind : keyBinds) {
		if (bind->Mode != ALWAYS_ON && (!bind || bind->Key == 0) || bind->Mode == ALWAYS_OFF)
			continue;

		input::get_key(*bind);
	}

	return true;
}

void init(const HMODULE module) {
	try {
		memory::get_all_modules();

		globals::m_local = *SIG("client.dll", "8B 0D ? ? ? ? 83 FF FF 74 07").self_offset(0x2).cast<c_local_player*>();

		interfaces::init();

		input::init();

		render::init();

		netvars::init();

		cfg::init();

		hooks::init();

		events::init();
	}
	catch (const std::exception& error)
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(
			0,
			error.what(),
			"Primeval | Error",
			MB_OK | MB_ICONEXCLAMATION
		);

		goto UNLOAD;
	}

	while (!GetAsyncKeyState(VK_DELETE)) {
		checkKeybinds();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	UNLOAD:
		events::undo();

		hooks::undo();

		input::undo();

		Sleep(200);

		FreeLibraryAndExitThread(module, 0);

}

BOOL WINAPI DllMain(
	const HMODULE module,
	const std::uintptr_t reason,
	const void* reserved
)
{
	if (reason != DLL_PROCESS_ATTACH)
		return TRUE;

	DisableThreadLibraryCalls(module);

	globals::m_module = module;

	if (const auto thread = CreateThread(
		nullptr,
		0,
		reinterpret_cast<LPTHREAD_START_ROUTINE>(init),
		module,
		0,
		nullptr
	))
	{
		CloseHandle(thread);
	}

	return TRUE;
}