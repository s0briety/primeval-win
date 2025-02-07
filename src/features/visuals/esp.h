#include "../../globals.h"

class c_esp : public c_singleton<c_esp> {
public:
	void CallEsp();
private:
	void RenderEsp(c_cs_player* player, int target, c_base_player* local, matrix3x4_t bones[128], vec3_t origin);
	void RenderBox(int target, vec2_t top, vec2_t bottom);
	void RenderHealth(int target, vec2_t top, vec2_t bottom, c_cs_player* player);
};
#define player_esp c_esp::instance()