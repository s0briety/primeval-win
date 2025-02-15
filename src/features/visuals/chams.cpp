#include "../features.h"
#include "../../hooks/hooks.h"

i_material* c_chams::create_material(std::string_view material_name, std::string_view shader_type, std::string_view material_data) {
	const auto key_values = reinterpret_cast<c_key_values*>(interfaces::m_mem_alloc->alloc(36u));

	key_values->init(shader_type.data());
	key_values->load_from_buffer(material_name.data(), material_data.data());

	return interfaces::m_material_system->create_material(material_name.data(), key_values);
}

void c_chams::override_material(int type, const col_t& clr, bool ignorez) {
	i_material* material = nullptr;

    switch (type) {
    case MATERIAL_TYPE_REGULAR: material = ignorez ? m_regular_z : m_regular; break;
    case MATERIAL_TYPE_FLAT: material = ignorez ? m_flat_z : m_flat; break;
    case MATERIAL_TYPE_WIREFRAME: material = ignorez ? m_wireframe_z : m_wireframe; break;
    case MATERIAL_TYPE_PLATINUM: material = ignorez ? m_platinum_z : m_platinum; break;
    case MATERIAL_TYPE_GLASS: material = ignorez ? m_glass_z : m_glass; break;
    case MATERIAL_TYPE_CRYSTAL: material = ignorez ? m_crystal_z : m_crystal; break;
    case MATERIAL_TYPE_GOLD: material = ignorez ? m_gold_z : m_gold; break;
    case MATERIAL_TYPE_DARK_CHROME: material = ignorez ? m_dark_chrome_z : m_dark_chrome; break;
    case MATERIAL_TYPE_PLASTIC: material = ignorez ? m_plastic_z : m_plastic; break;
    case MATERIAL_TYPE_GLOW: material = ignorez ? m_glow_z : m_glow; break;
    }

    material->color_modulate(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f);
    material->alpha_modulate(clr.a() / 255.f);

	if (const auto $envmaptint = material->find_var(_("$envmaptint"), nullptr, false)) {
		$envmaptint->set_value(vec3_t(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f));
	}

	interfaces::m_model_render->forced_material_override(material);
}

bool c_chams::on_draw_model(i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones) {
    if (!globals::config::chamsEnabled)
        return true;

    static const auto original = hooks::m_model_render->get_original<hooks::model_render::draw_model_execute::T>(hooks::model_render::draw_model_execute::index);

    const auto entity = reinterpret_cast<c_base_entity*>(interfaces::m_entity_list->get_client_entity(info.m_index));
    if (!entity || !entity->is_player())
        return true;

    const auto player = reinterpret_cast<c_cs_player*>(entity);
    if (!player->is_alive())
        return true;

    if (player->get_team() != globals::m_local->get_team() && globals::config::chamsTable[0].enabled) {
        ImColor color = globals::config::chamsTable[0].primary;

        e_material_type type = static_cast<e_material_type>(globals::config::chamsTable[0].type);

        if (type == MATERIAL_TYPE_GLOW) {
            ImColor secondary = globals::config::chamsTable[0].secondary;

            override_material(MATERIAL_TYPE_REGULAR, color, true);
            original(ecx, context, state, info, bones);
            // override_material(MATERIAL_TYPE_REGULAR, convertedColor, false);
            override_material(MATERIAL_TYPE_GLOW, secondary, true);
            original(ecx, context, state, info, bones);
            // override_material(MATERIAL_TYPE_GLOW, convertedSecondary, false);
            return true;
        }

        override_material(type, color, true);
        original(ecx, context, state, info, bones);
        // override_material(type, convertedColor, false);

    }
    else if (player->get_team() == globals::m_local->get_team() && globals::config::chamsTable[1].enabled && player != globals::m_local) {
        ImColor color = globals::config::chamsTable[1].primary;

        e_material_type type = static_cast<e_material_type>(globals::config::chamsTable[1].type);

        if (type == MATERIAL_TYPE_GLOW) {
            ImColor secondary = globals::config::chamsTable[1].secondary;

            override_material(MATERIAL_TYPE_REGULAR, color, true);
            original(ecx, context, state, info, bones);
            // override_material(MATERIAL_TYPE_REGULAR, convertedColor, false);
            override_material(MATERIAL_TYPE_GLOW, secondary, true);
            original(ecx, context, state, info, bones);
            // override_material(MATERIAL_TYPE_GLOW, convertedSecondary, false);
            return true;
        }

        override_material(type, color, true);
        original(ecx, context, state, info, bones);
        // override_material(type, convertedColor, false);

    }
    else if (player == globals::m_local && globals::config::chamsTable[2].enabled) {
        ImColor color = globals::config::chamsTable[2].primary;

        e_material_type type = static_cast<e_material_type>(globals::config::chamsTable[2].type);

        if (type == MATERIAL_TYPE_GLOW) {
            ImColor secondary = globals::config::chamsTable[2].secondary;

            override_material(MATERIAL_TYPE_REGULAR, color, true);
            original(ecx, context, state, info, bones);
            // override_material(MATERIAL_TYPE_REGULAR, convertedColor, false);
            override_material(MATERIAL_TYPE_GLOW, secondary, true);
            original(ecx, context, state, info, bones);
            // override_material(MATERIAL_TYPE_GLOW, convertedSecondary, false);
            return true;
        }

        override_material(type, color, true);
        original(ecx, context, state, info, bones);
        // override_material(type, convertedColor, false);

    }

    return true;
}