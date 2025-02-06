#include "../features.h"

void c_esp::CallEsp() {
	if (!globals::config::espEnabled || !globals::m_local || !interfaces::m_engine->is_in_game())
		return;

	for (int i = 1; i < interfaces::m_global_vars->m_max_clients; ++i)
	{
		const auto entity = reinterpret_cast<c_base_entity*>(interfaces::m_entity_list->get_client_entity(i));

		if (!entity || !entity->is_player())
			continue;

		c_cs_player* player = reinterpret_cast<c_cs_player*>(entity);

		if (!player || !player->is_alive() || player->is_dormant())
			continue;

		if (!globals::config::espSelf && player == globals::m_local)
			continue;

		if (player != globals::m_local 
			&& !globals::config::espTeammates
			&& (player->get_team() == globals::m_local->get_team()
			))
			continue;

		c_base_player* local = globals::m_local;

		vec3_t origin = player->get_render_origin();

		if (!globals::m_local->is_alive()) {
			c_base_player* observer_target = 
				reinterpret_cast
				<c_base_player*>
				(interfaces::m_entity_list->get_client_entity_from_handle
				(globals::m_local->get_observer_target()
				));

			local = observer_target;

			if (player == observer_target) {
				if (origin != observer_target->get_abs_origin())
					origin = observer_target->get_abs_origin();

				continue;
			}
		}

		if (local->get_abs_origin().dist_to(origin) < float(globals::config::espDistance))
			continue;

		matrix3x4_t bones[128];
		if (!player->setup_bones(bones, 128, 0x7FF00, interfaces::m_global_vars->m_cur_time))
			continue;

		// box esp

		vec3_t worldTop = bones[8].get_column(3) + vec3_t( 0.f, 0.f, 11.f );
		vec3_t worldBottom = origin - vec3_t( 0.f, 0.f, 9.f );

		vec2_t top;
		vec2_t bottom;

		worldTop.to_screen(top);
		worldBottom.to_screen(bottom);

		const float h = bottom.y - top.y;

		const float w = h * 0.3f;

		const auto left = static_cast<int>(top.x - w);
		const auto right = static_cast<int>(top.x + w);

		interfaces::m_surface->draw_set_color(255, 255, 255, 255);

		interfaces::m_surface->draw_outlined_rect(left, top.y, right, bottom.y);

		interfaces::m_surface->draw_set_color(0, 0, 0, 255);

		interfaces::m_surface->draw_outlined_rect(left - 1, top.y - 1, right + 1, bottom.y + 1);
		interfaces::m_surface->draw_outlined_rect(left + 1, top.y + 1, right - 1, bottom.y - 1);

		// health esp

		interfaces::m_surface->draw_outlined_rect(left - 6, top.y - 1, left - 3, bottom.y + 1);

		const float healthFrac = player->get_health() * 0.01f;

		interfaces::m_surface->draw_set_color((1.f - healthFrac) * 255, 255 * healthFrac, 0, 255);
		interfaces::m_surface->draw_filled_rect(left - 5, bottom.y - (h * healthFrac), left - 4, bottom.y);
	}
}