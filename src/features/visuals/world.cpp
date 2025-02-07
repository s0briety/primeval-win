#include "../features.h"

c_cvar* skyName;

float lastScale;

void c_world::CallNightMode() {

    if (!globals::config::nightMode || !globals::m_local) {
        if (nightModeSet && globals::m_local)
            UndoNightMode();

        return;
    }

    if (globals::config::nightScale != lastScale)
        UndoNightMode(true);

    if (nightModeSet && globals::config::nightMode)
        return;

    for (int i = 0; i < interfaces::m_entity_list->get_highest_entity_index(); i++) {
        i_client_entity* entity = interfaces::m_entity_list->get_client_entity(i);
        if (!entity)
            continue;

        if (entity->get_client_class()->m_class_id == C_ENV_TONEMAP_CONTROLLER) {
            *(float*)((uintptr_t)entity + 0x58) = globals::config::nightScale;
            *(float*)((uintptr_t)entity + 0x5C) = globals::config::nightScale;
            break;
        }
    }

    lastScale = globals::config::nightScale;

    /*
    if (!skyName)
        skyName = interfaces::m_cvar_system->find_var(FNV1A_RT("sv_skyname"));

    if (!originalSkybox)
    {
        if (skyName && skyName->get_name())
        {
            originalSkybox = skyName->get_string();
        }
    }
    */

    ApplyNightSkybox();
    Modulate(globals::config::nightScale, globals::config::nightScale, globals::config::nightScale);
    nightModeSet = true;
}

void c_world::Modulate(float r, float b, float g) {

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

void c_world::UndoNightMode(bool callback) {
    if (!nightModeSet || (globals::config::nightMode && !callback)) {
        return;
    }

    if (callback)
    {
        nightModeSet = false;
        return;
    }

    for (int i = 0; i < interfaces::m_entity_list->get_highest_entity_index(); i++) {
        i_client_entity* entity = interfaces::m_entity_list->get_client_entity(i);
        if (!entity)
            continue;

        if (entity->get_client_class()->m_class_id == C_ENV_TONEMAP_CONTROLLER) {
            *(float*)((uintptr_t)entity + 0x58) = 1.0f;
            *(float*)((uintptr_t)entity + 0x5C) = 1.0f;
            break;
        }
    }

    interfaces::m_engine->client_cmd_unrestricted("r_drawstaticprops 0");
    interfaces::m_engine->client_cmd_unrestricted("r_drawstaticprops 1");

    if (originalSkybox && !originalSkybox->empty())
    {
        SetSkybox(originalSkybox->c_str());
    }
    else
    {
        SetSkybox("sky_day02_05");
    }

    Modulate(1.0f, 1.0f, 1.0f);
    nightModeSet = false;
}

void c_world::overrideFOV(view_setup_t* view) {
    if (!view || !interfaces::m_engine->is_in_game() || !globals::config::worldFov)
        return;

    if (view->m_fov == globals::config::worldFov)
        return;

    view->m_fov = globals::config::worldFov;
}