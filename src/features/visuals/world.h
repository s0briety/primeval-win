#pragma once

#include "../../globals.h"

class c_world : public c_singleton<c_world> {
public:
	void CallNightMode();
	void Modulate(float r = 0.1f, float b = 0.1f, float g = 0.1f);
	void UndoNightMode(bool callback = false);
	void SetSkybox(const char* skyName);
private:
	void ApplyNightSkybox();
	std::string *originalSkybox;
	bool nightModeSet = false;
	using LoadSkyboxFn = void(__fastcall*)(const char*);
	LoadSkyboxFn load_skybox = nullptr;
};
#define world c_world::instance()