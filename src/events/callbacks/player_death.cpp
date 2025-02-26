#include "../events.h"

void events::player_death(i_game_event* event) {
	int victimIndex = event->get_int("userid");
	int attackerIndex = event->get_int("attacker");
	bool headshot = event->get_bool("headshot");

	if (attackerIndex == globals::m_local->get_info().m_user_id)
	{
		auto& shot = globals::game::user::m_ShotData[globals::game::user::ShotCount];

		shot.targetIndex = victimIndex;
		shot.kill = true;
		if (headshot)
			shot.hitgroup = 1;

		logging->aimbot_log(shot.shotIndex);
	}
}
