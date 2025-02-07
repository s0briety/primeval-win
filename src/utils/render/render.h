#pragma once

enum e_font_flags {
	FONT_NONE,
	FONT_CENTERED_X = 1 << 0,
	FONT_CENTERED_Y = 1 << 1,
	FONT_CENTERED = FONT_CENTERED_X | FONT_CENTERED_Y,
	FONT_DROP_SHADOW = 1 << 2,
	FONT_OUTLINE = 1 << 4
};

namespace render {
	void init();

	void DrawBorder(ImVec4 color, float thickness = 2.0f, float adjustment = 0.f, float rounding = 3.0f);
	
	void drawKeyBind(const char* label, KeyBind& parent, float v = 0.0f, float v_min = 0.0f, float v_max = 0.0f);

	bool DrawSwitch(const char* label, bool* v, bool bullet = true);

	bool drawSlider(const char* label, float* v, float v_min, float v_max, bool bullet = false, int precision = 0);

	void DrawColorPicker(const char* label, ImColor* color);

	extern std::mutex m_mutex;

	extern vec2_t m_screen_size;

}

namespace fonts {
	extern ImFont* Verdana_15;
	extern ImFont* Lucon_22;
}
