#include "../events.h"

void events::player_hurt(i_game_event* event) {
    int victimId = event->get_int("userid");
    int attackerId = event->get_int("attacker");
    int healthDamage = event->get_int("dmg_health");
	int armorDamage = event->get_int("dmg_armor");
	int health = event->get_int("health");
	int armor = event->get_int("armor");
	int hitgroup = event->get_int("hitgroup");

	int damage = healthDamage + armorDamage;

	float eventTime = interfaces::m_global_vars->m_cur_time;

	c_base_player* victim = reinterpret_cast<c_base_player*>(interfaces::m_entity_list->get_client_entity(interfaces::m_engine->get_player_for_user_id(victimId)));
	c_base_player* attacker = reinterpret_cast<c_base_player*>(interfaces::m_entity_list->get_client_entity(interfaces::m_engine->get_player_for_user_id(attackerId)));

	bool killed = (health <= 0 && armor <= 0 || !victim->is_alive());

	globals::game::ConsoleLog("Player hurt event", col_t::palette_t::white());

	if (attacker == globals::m_local) {
		for (auto& shot : globals::game::user::m_ShotData) {
			if (shot.processed && shot.impactPos != vec3_t(0, 0, 0) || eventTime - shot.time < 0.2f)
				continue;

			if (shot.attackerIndex == attackerId && shot.targetIndex == victimId) {
				shot.damage = damage;
				shot.hitgroup = hitgroup;
				shot.kill = killed;
				shot.processed = true;
			}
		}
	}
}