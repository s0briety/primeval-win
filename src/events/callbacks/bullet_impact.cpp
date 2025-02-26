#include "../events.h"

void events::bullet_impact(i_game_event* event) {
	int userid = event->get_int("userid");
	float x = event->get_float("x");
	float y = event->get_float("y");
	float z = event->get_float("z");

	if (userid == globals::m_local->get_info().m_user_id)
	{
		auto& shot = globals::game::user::m_ShotData[globals::game::user::ShotCount];

		shot.impactPos = vec3_t(x, y, z);
	}
}
