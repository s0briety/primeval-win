#include "../features.h"
#include "../../hooks/hooks.h"

void c_glow::DrawGlow() {
	if (!globals::config::glowEnabled)
		return;

	static i_glow_object_manager* GlowObjectManager = interfaces::m_glow_manager;

	for (int i = 0; i < GlowObjectManager->m_glow_object_definitions.size(); i++) {
		i_glow_object_manager::glow_object_t* glowEntity = &GlowObjectManager->m_glow_object_definitions[i];
		c_base_entity* Entity = glowEntity->m_entity;

		if (glowEntity->is_unused() || !Entity)
			continue;

		const auto player = reinterpret_cast<c_cs_player*>(glowEntity);

		switch (Entity->get_client_class()->m_class_id) {

		case 1:
			if (globals::config::glowEnabled)
				glowEntity->set(col_t::palette_t::light_purple(), 1);

			break;

		case 29:
			// if (Config->Visual.Filter.C4)
				// glowEntity->Set(Color(Config->Visual.Glow.C4Color));

			break;

		case 35:

			if (Entity->get_team() != globals::m_local->get_team() && globals::config::glowEnemies)
			{
				glowEntity->set(col_t::palette_t::pink(), 1);
			}
			else if (Entity->get_team() == globals::m_local->get_team() && globals::config::glowTeammates)
			{
				glowEntity->set(col_t::palette_t::pink(), 1);
			}
			else if (player == globals::m_local && globals::config::chamsSelf)
			{
				glowEntity->set(col_t::palette_t::pink(), 1);
			}

			break;

		case 39:
			if (globals::config::glowEnabled)
				glowEntity->set(col_t::palette_t::light_purple(), 1);

			break;

		case 41:
			if (globals::config::glowEnabled)
				glowEntity->set(col_t::palette_t::light_purple(), 1);

			break;

		case 105:
			if (globals::config::glowEnabled)
				glowEntity->set(col_t::palette_t::light_purple(), 1);
			break;

		default:
			if (globals::config::glowEnabled) {
				if (strstr(Entity->get_client_class()->m_network_name, "CWeapon"))
					glowEntity->set(col_t::palette_t::light_purple(), 1);
			}
			break;
		}
	}
}