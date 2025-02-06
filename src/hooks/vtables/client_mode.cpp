#include "../hooks.h"

void __stdcall hooks::client_mode::override_view::fn(view_setup_t* view) {
    static const auto original = m_client_mode->get_original<T>(index);

    original(view);
}

bool __fastcall hooks::client_mode::do_post_screen_space_effects::fn(void* ecx, void* edx, view_setup_t* view) {
    static const auto original = m_client_mode->get_original<T>(index);

    if (!globals::config::glowEnabled)
    {
        original(ecx, view);
        return true;
    }

    auto& glowTable = globals::config::glowTable;

    for (int i = 0; i < interfaces::m_glow_manager->m_glow_object_definitions.size(); ++i) {
        i_glow_object_manager::glow_object_t& glow_object = interfaces::m_glow_manager->m_glow_object_definitions[i];

        if (!glow_object.m_entity)
            continue;

        auto client_class = glow_object.m_entity->get_client_class();
        e_team_id team = glow_object.m_entity->get_team();
        e_team_id local_team = globals::m_local->get_team();

        int index = -1;
        col_t color;

        if (client_class->m_class_id == C_CS_PLAYER) {
            if (glowTable[2].enabled && glow_object.m_entity == globals::m_local) {
                color = globals::ImColorToCol_T(glowTable[2].primary);
                index = 2;
            }
            else if (glowTable[1].enabled && team == local_team && glow_object.m_entity != globals::m_local) {
                color = globals::ImColorToCol_T(glowTable[1].primary);
                index = 1;
            }
            else if (glowTable[0].enabled && team != local_team) {
                color = globals::ImColorToCol_T(glowTable[0].primary);
                index = 0;
            }

            if (index != -1) {
                glow_object.set(color, globals::config::glowTable[index].type);
            }
        }
    }

    original(ecx, view);
    return true;
}
