#pragma once

#include "../../globals.h"

class c_world : public c_singleton<c_world> {
public:
	void ApplyNightMode();
	void ModulateProps(float r = 0.1f, float b = 0.1f, float g = 0.1f);
	void UndoNightMode();
	void SetSkybox(const char* skyName);
private:
	void ApplyNightSkybox();
	bool nightModeSet = false;
	using LoadSkyboxFn = void(__fastcall*)(const char*); // Define function pointer type
	LoadSkyboxFn load_skybox = nullptr;
};
#define world c_world::instance()