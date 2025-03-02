#include "../../globals.h"

class c_esp : public c_singleton<c_esp> {
public:
	void CallEsp();
private:
	void AdjustColorTone(int& r, int& g, int& b, float brightnessFactor, float saturationFactor);
	void RenderEsp(c_cs_player* player, int target, c_base_player* local, matrix3x4_t bones[128], vec3_t origin);
	void RenderBox(vec2_t top, vec2_t bottom, ImColor primary, ImColor secondary);
	void RenderFlags(c_cs_player* player, vec2_t top, vec2_t bottom);
	void RenderHealth(const float health, vec2_t top, vec2_t bottom, ImColor primary);
	void RenderHealthBar(const float health, vec2_t top, vec2_t bottom, ImColor primary);
};
#define player_esp c_esp::instance()