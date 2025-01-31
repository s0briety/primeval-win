#pragma once
#include "../../globals.h"
class c_glow : public c_singleton<c_glow> {
public:
	void DrawGlow();
};
#define glow c_glow::instance()