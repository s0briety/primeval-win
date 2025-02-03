#include "../features.h"

void c_world::ApplyNightMode() {
    if (!globals::config::nightMode || nightModeSet)
        UndoNightMode();
        return;

    for (int i = 0; i < interfaces::m_entity_list->get_highest_entity_index(); i++) {
        i_client_entity* entity = interfaces::m_entity_list->get_client_entity(i);
        if (!entity)
            continue;

        if (entity->get_client_class()->m_class_id == C_ENV_TONEMAP_CONTROLLER) {
            *(float*)((uintptr_t)entity + 0x58) = 0.1f;
            *(float*)((uintptr_t)entity + 0x5C) = 0.1f;
            break;
        }
    }

    ApplyNightSkybox();
    ModulateProps();
    nightModeSet = true;
}

void c_world::ModulateProps(float r, float b, float g) {
    // interfaces::m_input->get_user_cmd(FNV1A("r_drawspecificstaticprop 0"));

    for (int i = interfaces::m_material_system->first_material();
        i != interfaces::m_material_system->invalid_material();
        i = interfaces::m_material_system->next_material(i)) {
        i_material* material = interfaces::m_material_system->get_material(i);
        if (!material || !material->get_name())
            continue;

        if (strstr(material->get_texture_group_name(), "World") ||
            strstr(material->get_texture_group_name(), "StaticProp")) {
            material->color_modulate(r, b, g);
        }
    }
}

void c_world::SetSkybox(const char* skyName) {
    if (!load_skybox) {
        load_skybox = SIG("engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45").cast<LoadSkyboxFn>();
    }

    if (load_skybox) {
        load_skybox(skyName);
    }
}

void c_world::ApplyNightSkybox() {
    SetSkybox("sky_csgo_night02b");
}

void c_world::UndoNightMode() {
    if (globals::config::nightMode || !nightModeSet)
        return;

    for (int i = 0; i < interfaces::m_entity_list->get_highest_entity_index(); i++) {
        i_client_entity* entity = interfaces::m_entity_list->get_client_entity(i);
        if (!entity)
            continue;

        if (entity->get_client_class()->m_class_id == C_ENV_TONEMAP_CONTROLLER) {
            *(float*)((uintptr_t)entity + 0x58) = 1.f;
            *(float*)((uintptr_t)entity + 0x5C) = 1.f;
            break;
        }
    }

    ModulateProps(1.0f, 1.0f, 1.0f);
    nightModeSet = false;
}
