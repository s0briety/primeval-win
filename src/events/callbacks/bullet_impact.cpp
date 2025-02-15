#include "../events.h"

void events::bullet_impact(i_game_event* event)
{
	int userID = event->get_int("userid");

	float x = event->get_float("x");
	float y = event->get_float("y");
	float z = event->get_float("z");

	float eventTime = interfaces::m_global_vars->m_cur_time;

	globals::game::ConsoleLog("Bullet impact event", col_t::palette_t::white());

	if (userID == interfaces::m_engine->get_local_player())
	{
		for (auto& shot : globals::game::user::m_ShotData) {
			if (shot.processed && shot.damage || eventTime - shot.time < 0.2f)
				continue;

			if (shot.attackerIndex == userID) {
				shot.impactPos = vec3_t(x, y, z);
				shot.processed = true;
			}
		}
	}

}