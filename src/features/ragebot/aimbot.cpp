#include "../features.h"
#include <sstream>


float c_aimbot::CalculateDistance(const vec3_t& target, const vec3_t& me)
{
    vec3_t delta = target - me;
    return std::sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
}

void c_aimbot::run(c_user_cmd* cmd)
{
    if (!globals::config::aimbotEnabled || !globals::m_local || !globals::m_local->is_alive())
        return;

    if (!globals::config::aimbotAutoShoot && !globals::config::silentAimEnabled && !cmd->m_buttons.has(IN_ATTACK))
        return;

    c_base_combat_weapon* activeWeapon;

    activeWeapon = globals::m_local->get_active_weapon();

   int weaponType;

    weaponType = activeWeapon->get_cs_weapon_data()->m_weapon_type;

    switch (weaponType)
    {
    case WEAPON_TYPE_MACHINE_GUN:
    case WEAPON_TYPE_SMG:
    case WEAPON_TYPE_RIFLE:
    case WEAPON_TYPE_SHOTGUN:
    case WEAPON_TYPE_SNIPER:
    case WEAPON_TYPE_PISTOL:
    {
        if (!activeWeapon || !activeWeapon->get_ammo())
            return;

        break;
    }

    default:
        return;
    }

    vec3_t bestTargetAngle{};
    c_cs_player* bestTarget = nullptr;
    float bestMetric = FLT_MAX;
    vec3_t CurrentAimPunch{};

    for (int i = 1; i <= interfaces::m_entity_list->get_max_entities(); ++i)
    {
        i_client_entity* entity;
        entity = interfaces::m_entity_list->get_client_entity(i);

        if (!entity || !entity->get_base_entity()->is_player())
            continue;

        c_cs_player* player;

        player = reinterpret_cast<c_cs_player*>(entity);

        if (!player || player == globals::m_local || !player->is_alive() || player->is_dormant() || player->get_team() == globals::m_local->get_team())
            continue;

        matrix3x4_t bones[128];
        if (!player->setup_bones(bones, 128, 256, interfaces::m_global_vars->m_cur_time))
            continue;

        const model_t* model;
        model = player->get_model();

        if (!model)
            continue;


        studiohdr_t* studioHdr = interfaces::m_model_info->get_studio_model(model);
        if (!studioHdr)
            continue;

        const mstudiohitboxset_t* hitboxSet;
        hitboxSet = studioHdr->get_hitbox_set(0);

        if (!hitboxSet)
            continue;

        vec3_t localEyePos;
        localEyePos = globals::m_local->get_eye_pos();

        for (int j = 0; j < 8; ++j)
        {
            if (!globals::config::hitboxTable[j].isEnabled)
                continue;

            for (int k = 0; k < 2; ++k)
            {

                const mstudiobbox_t* hitbox;
                hitbox = hitboxSet->get_hitbox(globals::config::hitboxTable[j].ref[k]);

                if (!hitbox)
                    continue;

                vec3_t hitboxCenter;
                hitboxCenter.x = (hitbox->m_obb_min.x + hitbox->m_obb_max.x) * 0.5f;
                hitboxCenter.y = (hitbox->m_obb_min.y + hitbox->m_obb_max.y) * 0.5f;
                hitboxCenter.z = (hitbox->m_obb_min.z + hitbox->m_obb_max.z) * 0.5f;

                vec3_t transformedCenter;
                transformedCenter.x = bones[hitbox->m_bone].m_value[0][0] * hitboxCenter.x + bones[hitbox->m_bone].m_value[0][1] * hitboxCenter.y + bones[hitbox->m_bone].m_value[0][2] * hitboxCenter.z + bones[hitbox->m_bone].m_value[0][3];
                transformedCenter.y = bones[hitbox->m_bone].m_value[1][0] * hitboxCenter.x + bones[hitbox->m_bone].m_value[1][1] * hitboxCenter.y + bones[hitbox->m_bone].m_value[1][2] * hitboxCenter.z + bones[hitbox->m_bone].m_value[1][3];
                transformedCenter.z = bones[hitbox->m_bone].m_value[2][0] * hitboxCenter.x + bones[hitbox->m_bone].m_value[2][1] * hitboxCenter.y + bones[hitbox->m_bone].m_value[2][2] * hitboxCenter.z + bones[hitbox->m_bone].m_value[2][3];

                if (globals::config::multipointTable[j].isEnabled)
                {
                    float multipointScale = (j == HITBOX_HEAD || j == HITBOX_NECK) ? globals::config::multipointHead : globals::config::multipointBody;
                    transformedCenter.x += multipointScale;
                    transformedCenter.y += multipointScale;
                    transformedCenter.z += multipointScale;
                }

                vec3_t aimPunch = globals::m_local->get_aim_punch_angle().vector();

                vec3_t angleToTarget = (transformedCenter - localEyePos) - aimPunch;

                float metric = 0.0f;
                switch (globals::config::aimbotTargetSelection)
                {
                case 0:
                    metric = CalculateDistance(transformedCenter, localEyePos);
                    break;
                case 1:
                    metric = static_cast<float>(player->get_health());
                    break;
                case 2:
                    // metric = calculateHitchance(activeWeapon, localEyePos, transformedCenter, aimPunch);
                    metric = CalculateDistance(transformedCenter, localEyePos);
                    break;
                }

                if (metric < bestMetric)
                {
                    bestMetric = metric;
                    bestTargetAngle = angleToTarget;
                    CurrentAimPunch = aimPunch;
                    bestTarget = player;
                }
            }
        }
    }

    if (bestTarget)
    {
        if (globals::config::aimbotAutoscope && weaponType == WEAPON_TYPE_SNIPER && !globals::m_local->is_scoped())
        {
            cmd->m_buttons.add(IN_ZOOM);
        }

        qangle_t finalAngles = bestTargetAngle.angle();

        finalAngles = finalAngles - (CurrentAimPunch.angle() + CurrentAimPunch.angle());

        cmd->m_view_angles = finalAngles;

        if (globals::config::aimbotAutoShoot)
        {
            if (cmd->m_buttons.has(IN_ATTACK))
                cmd->m_buttons.remove(IN_ATTACK);

            cmd->m_buttons.add(IN_ATTACK);
        }

    }
}
