#pragma once
#include "../fonts/fonts.h"
#include "../icons/icons.h"

enum e_font_flags {
	FONT_NONE,
	FONT_CENTERED_X = 1 << 0,
	FONT_CENTERED_Y = 1 << 1,
	FONT_CENTERED = FONT_CENTERED_X | FONT_CENTERED_Y,
	FONT_DROP_SHADOW = 1 << 2,
	FONT_OUTLINE = 1 << 4
};

namespace fonts {
	extern bool initialized;
	extern ImFont* Verdana_15;
	extern ImFont* TitleFont;
	extern ImFont* smallest_pixel_14;
}

namespace textures {
	extern bool initialized;
	extern ImTextureID logo;
	extern ImTextureID ragetab;
	extern ImTextureID aaTab;
	extern ImTextureID visualsTab;
	extern ImTextureID miscTab;
	extern ImTextureID configTab;
}

namespace render {
	ImTextureID LoadTextureFromMemory(IDirect3DDevice9* device, unsigned char* imageData, int imageSize);

	void init();

	void DrawBorder(ImVec4 color, float thickness = 2.0f, float adjustment = 0.f, float rounding = 3.0f);
	
	bool DrawButton(const char* label, bool* value, const ImVec2& pos, ImVec2 size_arg = ImVec2(12, 12), ImGuiButtonFlags flags = 0);

	bool DrawCombo(const char* label, const char* items[], ImVec2 pos, float width, int selectedIndexes[], int item_count, int max_selected = 0);
	bool DrawCombo(const char* label, const char* items[], ImVec2 pos, float width, Hitbox hitboxTable[], int item_count, int max_selected = 0);
	bool DrawCombo(const char* label, const char* items[], ImVec2 pos, float width, Combostruct Table[], int item_count, int max_selected = 0);

	bool PopupColorPicker(const char* label, ImColor* color, ImVec2 pos);
	bool PopupColorPicker(const char* label, ImVec4* color, ImVec2 pos);
	bool ColorSlider(const char* label, ImColor* color, float min, float max, ImVec2 pos);

	bool DrawSlider(const char* label, float* value, float min, float max, ImVec2 pos, float slider_width = 250.f);
	bool DrawSlider(const char* label, int* value, int min, int max, ImVec2 pos, float slider_width = 250.f);

	void DrawKeybind(const char* label, KeyBind& parent, ImVec2 pos, float v = 0.0f, float v_min = 0.0f, float v_max = 0.0f, int addition = 0);

	extern std::mutex m_mutex;

	extern vec2_t m_screen_size;

}

namespace render_game {
	extern void DrawPlayerBox(int r, int g, int b, int a, int rr, int gg, int bb, int aa, const int left, const int right, float top, float bottom);
	extern void DrawHealthBar(const float healthFrac, const float h, int r, int g, int b, int a, const int left, const int right, float top, float bottom);
	extern void DrawText2D(ImFont* font, float x, float y, int r, int g, int b, int a, const char* text);
}