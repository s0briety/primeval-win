#include "../events.h"

void events::player_hurt(i_game_event* event) {
	int victimIndex = event->get_int("userid");
	int attackerIndex = event->get_int("attacker");
	int damageHealth = event->get_int("dmg_health");
	int damageArmor = event->get_int("dmg_armor");
	int hitgroup = event->get_int("hitgroup");

	if (attackerIndex == globals::m_local->get_info().m_user_id)
	{
		auto& shot = globals::game::user::m_ShotData[globals::game::user::ShotCount];

		shot.damage = damageHealth + damageArmor;
		shot.hitgroup = hitgroup;
		shot.kill = false;
	}
}

