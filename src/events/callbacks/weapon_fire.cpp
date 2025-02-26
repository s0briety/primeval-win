#include "../events.h"

void events::weapon_fire(i_game_event* event)
{
	int attackerIndex = event->get_int("userid");

	if (attackerIndex == globals::m_local->get_info().m_user_id)
	{
		globals::game::user::ShotCount++;

		globals::ShotData shot;

		shot.shotIndex = globals::game::user::ShotCount;
		shot.attackerIndex = attackerIndex;
		shot.time = interfaces::m_global_vars->m_cur_time;
		shot.processed = false;

		globals::game::user::m_ShotData[shot.shotIndex] = shot;
	}
}