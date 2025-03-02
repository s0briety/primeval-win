#include "../features.h"

enum target_type {
	ENEMY,
	TEAMMATE,
	SELF
};

void c_esp::AdjustColorTone(int& r, int& g, int& b, float brightnessFactor, float saturationFactor) {
	r = std::clamp(static_cast<int>(r * brightnessFactor), 0, 255);
	g = std::clamp(static_cast<int>(g * brightnessFactor), 0, 255);
	b = std::clamp(static_cast<int>(b * brightnessFactor), 0, 255);

	int maxChannel = std::max({ r, g, b });
	if (maxChannel > 0) {
		float scale = 255.0f / maxChannel;
		r = std::clamp(static_cast<int>(r * scale * saturationFactor), 0, 255);
		g = std::clamp(static_cast<int>(g * scale * saturationFactor), 0, 255);
		b = std::clamp(static_cast<int>(b * scale * saturationFactor), 0, 255);
	}
}

void c_esp::RenderHealth(const float health, vec2_t top, vec2_t bottom, ImColor primary)
{
	const float h = bottom.y - top.y;

	const float w = h * 0.3f;

	const auto left = (top.x - w) - 5.f;

	std::string healthString = std::to_string(static_cast<int>(health));
	const char* HealthString = healthString.c_str();

	int r = primary.Value.x;
	int g = primary.Value.y;
	int b = primary.Value.z;
	int a = primary.Value.w;

	if (health > 0)
		render_game::DrawText2D(fonts::smallest_pixel_14, left, h, r, g, b, a, HealthString);
}

void c_esp::RenderHealthBar(const float health, vec2_t top, vec2_t bottom, ImColor primary) {
	const float h = bottom.y - top.y;

	const float w = h * 0.3f;

	const auto left = top.x - w;
	const auto right = top.x + w;

	int r = 255, g = 255, b = 255, a = 255;

	if (primary)
	{
		r = primary.Value.x * 255.f;
		g = primary.Value.y * 255.f;
		b = primary.Value.z * 255.f;
		a = primary.Value.w * 255.f;
	}

	const float healthFrac = health * 0.01f;

	float brightnessFactor = 0.7f + (healthFrac * 0.3f);
	float saturationFactor = 0.8f + (healthFrac * 0.2f);

	AdjustColorTone(r, g, b, brightnessFactor, saturationFactor);

	render_game::DrawHealthBar(healthFrac, h, r, g, b, a, left, right, top.y, bottom.y);
}

void c_esp::RenderBox(vec2_t top, vec2_t bottom, ImColor primary, ImColor secondary) {
	const float h = bottom.y - top.y;

	const float w = h * 0.3f;

	const auto left = static_cast<int>(top.x - w);
	const auto right = static_cast<int>(top.x + w);

	int r = 255, g = 255, b = 255, a = 255;
	int rr = 0, gg = 0, bb = 0, aa = 255;

	if (primary)
	{
		r = primary.Value.x * 255.f;
		g = primary.Value.y * 255.f;
		b = primary.Value.z * 255.f;
		a = primary.Value.w * 255.f;
	}

	if (secondary)
	{
		rr = secondary.Value.x * 255.f;
		gg = secondary.Value.y * 255.f;
		bb = secondary.Value.z * 255.f;
		aa = secondary.Value.w * 255.f;
	}

	render_game::DrawPlayerBox(r, g, b, a, rr, gg, bb, aa, left, right, top.y, bottom.y);
}

void c_esp::RenderEsp(c_cs_player* player, int target, c_base_player* local, matrix3x4_t bones[128], vec3_t origin) {
	vec3_t worldTop = bones[8].get_column(3) + vec3_t(0.f, 0.f, 11.f);
	vec3_t worldBottom = origin - vec3_t(0.f, 0.f, 9.f);

	vec2_t top;
	vec2_t bottom;

	worldTop.to_screen(top);
	worldBottom.to_screen(bottom);

	const float health = player->get_health();

	switch (target)
	{
	case ENEMY:
		if (!globals::config::espConfig.enemy.enabled || local->get_abs_origin().dist_to(origin) > float(globals::config::espConfig.enemy.distance * 20.f))
			return;

		if (globals::config::espConfig.enemy.box)
			RenderBox(top, bottom, globals::config::espConfig.enemy.boxPrimary, globals::config::espConfig.enemy.boxSecondary);

		if (globals::config::espConfig.enemy.healthText)
			RenderHealth(health, top, bottom, globals::config::espConfig.enemy.healthTextPrimary);

		if (globals::config::espConfig.enemy.healthBar)
			RenderHealthBar(health, top, bottom, globals::config::espConfig.enemy.HealthPrimary);
		break;
	case TEAMMATE:
		if (!globals::config::espConfig.teammate.enabled || local->get_abs_origin().dist_to(origin) > float(globals::config::espConfig.teammate.distance * 100.f))
			return;

		if (globals::config::espConfig.teammate.box)
			RenderBox(top, bottom, globals::config::espConfig.teammate.boxPrimary, globals::config::espConfig.teammate.boxSecondary);

		if (globals::config::espConfig.teammate.healthText)
			RenderHealth(health, top, bottom, globals::config::espConfig.teammate.healthTextPrimary);

		if (globals::config::espConfig.teammate.healthBar)
			RenderHealthBar(health, top, bottom, globals::config::espConfig.teammate.HealthPrimary);
		break;
	case SELF:
		if (!globals::config::espConfig.self.enabled || local->get_abs_origin().dist_to(origin) > float(globals::config::espConfig.self.distance * 100.f))
			return;

		if (globals::config::espConfig.self.box)
			RenderBox(top, bottom, globals::config::espConfig.self.boxPrimary, globals::config::espConfig.self.boxSecondary);

		if (globals::config::espConfig.self.healthText)
			RenderHealth(health, top, bottom, globals::config::espConfig.self.healthTextPrimary);

		if (globals::config::espConfig.self.healthBar)
			RenderHealthBar(health, top, bottom, globals::config::espConfig.self.HealthPrimary);
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