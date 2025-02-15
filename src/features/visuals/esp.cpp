#include "../features.h"

enum target_type {
	ENEMY,
	TEAMMATE,
	SELF
};

void c_esp::RenderHealth(int target, vec2_t top, vec2_t bottom, c_cs_player* player) {
	const float h = bottom.y - top.y;

	const float w = h * 0.3f;

	const auto left = static_cast<int>(top.x - w);
	const auto right = static_cast<int>(top.x + w);

	int r = 255, g = 255, b = 255, a = 255;

	switch (target)
	{
	case ENEMY:
		if (globals::config::espConfig.enemy.HealthPrimary.Value.w)
		{
			r = globals::config::espConfig.enemy.HealthPrimary.Value.x * 255.f;
			g = globals::config::espConfig.enemy.HealthPrimary.Value.y * 255.f;
			b = globals::config::espConfig.enemy.HealthPrimary.Value.z * 255.f;
			a = globals::config::espConfig.enemy.HealthPrimary.Value.w * 255.f;
		}
		break;

	case TEAMMATE:
		if (globals::config::espConfig.teammate.HealthPrimary.Value.w)
		{
			r = globals::config::espConfig.teammate.HealthPrimary.Value.x * 255.f;
			g = globals::config::espConfig.teammate.HealthPrimary.Value.y * 255.f;
			b = globals::config::espConfig.teammate.HealthPrimary.Value.z * 255.f;
			a = globals::config::espConfig.teammate.HealthPrimary.Value.w * 255.f;
		}
		break;

	case SELF:
		if (globals::config::espConfig.self.HealthPrimary.Value.w)
		{
			r = globals::config::espConfig.self.HealthPrimary.Value.x * 255.f;
			g = globals::config::espConfig.self.HealthPrimary.Value.y * 255.f;
			b = globals::config::espConfig.self.HealthPrimary.Value.z * 255.f;
			a = globals::config::espConfig.self.HealthPrimary.Value.w * 255.f;
		}
		break;
	}

	const float healthFrac = player->get_health() * 0.01f;

	r = (1.f - healthFrac) * r;
	g = healthFrac * g;

	interfaces::m_surface->draw_set_color(0, 0, 0, 255);
	interfaces::m_surface->draw_outlined_rect(left - 6, top.y - 1, left - 3, bottom.y + 1);

	interfaces::m_surface->draw_set_color(r, g, b, a);
	interfaces::m_surface->draw_filled_rect(left - 5, bottom.y - (h * healthFrac), left - 4, bottom.y);

	std::string healthString = std::to_string(static_cast<int>(healthFrac * 100.f));
	const char* HealthString = healthString.c_str();

	int width = 0;
	int height = 0;
	interfaces::m_engine->get_screen_size(width, height);
	D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);

}

void c_esp::RenderBox(int target, vec2_t top, vec2_t bottom) {
	const float h = bottom.y - top.y;

	const float w = h * 0.3f;

	const auto left = static_cast<int>(top.x - w);
	const auto right = static_cast<int>(top.x + w);

	int r = 255, g = 255, b = 255, a = 255;
	int rr = 0, gg = 0, bb = 0, aa = 255;

	switch (target)
	{
	case ENEMY:
		if (globals::config::espConfig.enemy.boxPrimary.Value.w)
		{
			r = globals::config::espConfig.enemy.boxPrimary.Value.x * 255.f;
			g = globals::config::espConfig.enemy.boxPrimary.Value.y * 255.f;
			b = globals::config::espConfig.enemy.boxPrimary.Value.z * 255.f;
			a = globals::config::espConfig.enemy.boxPrimary.Value.w * 255.f;
		}

		if (globals::config::espConfig.enemy.boxSecondary.Value.w * 255.f)
		{
			rr = globals::config::espConfig.enemy.boxSecondary.Value.x * 255.f;
			gg = globals::config::espConfig.enemy.boxSecondary.Value.y * 255.f;
			bb = globals::config::espConfig.enemy.boxSecondary.Value.z * 255.f;
			aa = globals::config::espConfig.enemy.boxSecondary.Value.w * 255.f;
		}
		break;

	case TEAMMATE:
		if (globals::config::espConfig.teammate.boxPrimary.Value.w)
		{
			r = globals::config::espConfig.teammate.boxPrimary.Value.x * 255.f;
			g = globals::config::espConfig.teammate.boxPrimary.Value.y * 255.f;
			b = globals::config::espConfig.teammate.boxPrimary.Value.z * 255.f;
			a = globals::config::espConfig.teammate.boxPrimary.Value.w * 255.f;
		}

		if (globals::config::espConfig.teammate.boxSecondary.Value.w)
		{
			rr = globals::config::espConfig.teammate.boxSecondary.Value.x * 255.f;
			gg = globals::config::espConfig.teammate.boxSecondary.Value.y * 255.f;
			bb = globals::config::espConfig.teammate.boxSecondary.Value.z * 255.f;
			aa = globals::config::espConfig.teammate.boxSecondary.Value.w * 255.f;
		}
		break;

	case SELF:
		if (globals::config::espConfig.self.boxPrimary.Value.w)
		{
			r = globals::config::espConfig.self.boxPrimary.Value.x * 255.f;
			g = globals::config::espConfig.self.boxPrimary.Value.y * 255.f;
			b = globals::config::espConfig.self.boxPrimary.Value.z * 255.f;
			a = globals::config::espConfig.self.boxPrimary.Value.w * 255.f;
		}

		if (globals::config::espConfig.self.boxSecondary.Value.w)
		{
			rr = globals::config::espConfig.self.boxSecondary.Value.x * 255.f;
			gg = globals::config::espConfig.self.boxSecondary.Value.y * 255.f;
			bb = globals::config::espConfig.self.boxSecondary.Value.z * 255.f;
			aa = globals::config::espConfig.self.boxSecondary.Value.w * 255.f;
		}
		break;
	}

	interfaces::m_surface->draw_set_color(r, g, b, a);

	interfaces::m_surface->draw_outlined_rect(left, top.y, right, bottom.y);

	interfaces::m_surface->draw_set_color(rr, gg, bb, aa);

	interfaces::m_surface->draw_outlined_rect(left - 1, top.y - 1, right + 1, bottom.y + 1);
	interfaces::m_surface->draw_outlined_rect(left + 1, top.y + 1, right - 1, bottom.y - 1);
}

void c_esp::RenderEsp(c_cs_player* player, int target, c_base_player* local, matrix3x4_t bones[128], vec3_t origin) {
	vec3_t worldTop = bones[8].get_column(3) + vec3_t(0.f, 0.f, 11.f);
	vec3_t worldBottom = origin - vec3_t(0.f, 0.f, 9.f);

	vec2_t top;
	vec2_t bottom;

	worldTop.to_screen(top);
	worldBottom.to_screen(bottom);

	switch (target)
	{
	case ENEMY:
		if (!globals::config::espConfig.enemy.enabled || local->get_abs_origin().dist_to(origin) > float(globals::config::espConfig.enemy.distance * 100.f))
			return;

		if (globals::config::espConfig.enemy.box)
			RenderBox(target, top, bottom);

		if (globals::config::espConfig.enemy.healthBar)
			RenderHealth(target, top, bottom, player);
		break;
	case TEAMMATE:
		if (!globals::config::espConfig.teammate.enabled || local->get_abs_origin().dist_to(origin) > float(globals::config::espConfig.teammate.distance * 100.f))
			return;

		if (globals::config::espConfig.teammate.box)
			RenderBox(target, top, bottom);

		if (globals::config::espConfig.teammate.healthBar)
			RenderHealth(target, top, bottom, player);
		break;
	case SELF:
		if (!globals::config::espConfig.self.enabled || local->get_abs_origin().dist_to(origin) > float(globals::config::espConfig.self.distance * 100.f))
			return;

		if (globals::config::espConfig.self.box)
			RenderBox(target, top, bottom);

		if (globals::config::espConfig.self.healthBar)
			RenderHealth(target, top, bottom, player);
		break;
	default:
		break;
	}
}

void c_esp::CallEsp() {
	if (!globals::config::espEnabled || !globals::m_local || !interfaces::m_engine->is_in_game())
		return;

	for (const auto& data : gameEntityHandler->EntityStorage)
	{
		if (!data.entity || data.type != globals::EntityType::Player)
			continue;

		c_cs_player* player = reinterpret_cast<c_cs_player*>(data.entity);

		int target = ENEMY;

		if (!player || !player->is_alive() || player->is_dormant())
			continue;

		if (!globals::config::espConfig.enemy.enabled
			&& (player->is_enemy(globals::m_local)
			))
			continue;

		if (player != globals::m_local 
			&& !globals::config::espConfig.teammate.enabled
			&& (player->get_team() == globals::m_local->get_team()
			))
			continue;
		else if (player != globals::m_local && player->get_team() == globals::m_local->get_team())
		{
			target = TEAMMATE;
		}

		if (!globals::config::espConfig.self.enabled && player == globals::m_local)
		{
			continue;
		}
		else if (player == globals::m_local)
		{
			target = SELF;
		}

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

		matrix3x4_t bones[128];
		if (!player->setup_bones(bones, 128, 0x7FF00, interfaces::m_global_vars->m_cur_time))
			continue;

		RenderEsp(player, target, local, bones, origin);
	}
}