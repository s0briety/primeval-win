#include "../features.h"
#include <sstream>

float c_aimbot::CalculateDistance(const vec3_t& target, const vec3_t& me)
{
    vec3_t delta = target - me;
    return delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
}

void c_aimbot::CalculateSpread(float inaccuracy, float spread, float& spreadX, float& spreadY) {
    float rand1 = math::random(0.f, 1.f);
    float rand2 = math::random(0.f, 1.f);
    float randPi1 = math::random(0.f, math::m_pi * 2);
    float randPi2 = math::random(0.f, math::m_pi * 2);

    spreadX = cos(randPi1) * (rand1 * inaccuracy) + cos(randPi2) * (rand2 * spread);
    spreadY = sin(randPi1) * (rand1 * inaccuracy) + sin(randPi2) * (rand2 * spread);
}

vec3_t c_aimbot::PredictTargetPosition(const vec3_t& targetPos, const vec3_t& targetVel) {
    float velocity = targetVel.length();

    vec3_t direction = targetVel.normalized();

    float predictionTime = interfaces::m_global_vars->m_interval_per_tick * 0.5f;
    vec3_t predictedPos = targetPos + (direction * velocity * predictionTime);

    globals::game::ConsoleLog(
        "Predicted Position: (" + std::to_string(predictedPos.x) + ", " +
        std::to_string(predictedPos.y) + ", " + std::to_string(predictedPos.z) + ")",
        globals::config::secondaryColor
    );
    globals::game::ConsoleLog(
        "Target Velocity: " + std::to_string(velocity),
        globals::config::secondaryColor
    );

    return predictedPos;
}

bool c_aimbot::TraceToTarget(const vec3_t& eyePos, const vec3_t& position, const vec3_t& spreadDirection, c_base_player* target) {
    vec3_t endPos = position + (spreadDirection * 8192.f);

    c_game_trace trace;
    ray_t ray(eyePos, endPos);
    c_trace_filter filter(globals::m_local);
    interfaces::m_trace_system->trace_ray(ray, MASK_SHOT | CONTENTS_HITBOX, &filter, &trace);

    return trace.did_hit_non_world_entity() && trace.get_entity_index() && interfaces::m_entity_list->get_client_entity(trace.get_entity_index()) == target;
}

float c_aimbot::CalculateHitChance(c_base_combat_weapon* weapon, c_base_player* target, qangle_t AngleToTarget) {
    int weaponType = weapon->get_cs_weapon_data()->m_weapon_type;

    if (!interfaces::m_engine->is_in_game() || !globals::m_local || !weaponType || !target)
        return 0.f;

    if (weaponType == WEAPON_TYPE_KNIFE || weaponType == WEAPON_TYPE_GRENADE)
        return 0.f;

    globals::game::user::TryingToShoot = true;

    float inaccuracy = weapon->get_inaccuracy();
    float spread = weapon->get_spread();

    if (inaccuracy == 0.0f)
        return true;

    int hitCount = 0;
    int totalTraces = 256;
    float spreadPenalty = 1.0f;

    if (globals::game::user::lastScopeTime > 0.f && (interfaces::m_global_vars->m_cur_time - globals::game::user::lastScopeTime) <= 0.25f) {
        spreadPenalty = 1.25f;
    }

    vec3_t eyePos = globals::m_local->get_eye_pos();

    vec3_t targetVel = target->get_velocity();
    vec3_t targetPos = target->get_origin();

//  vec3_t predictedTargetPos = PredictTargetPosition(targetPos, targetVel);

    for (int i = 0; i < totalTraces; i++) {
        float spreadX, spreadY;

        CalculateSpread(inaccuracy * spreadPenalty, spread * spreadPenalty, spreadX, spreadY);

        vec3_t forward, right, up;
        qangle_t viewAngles = globals::m_cur_cmd->m_view_angles;
        math::AngleVectors(viewAngles, &forward, &right, &up);

        vec3_t spreadDirection = forward + (right * spreadX) + (up * spreadY);
        spreadDirection.normalize();

        if (TraceToTarget(eyePos, targetPos, spreadDirection, target)) {
            hitCount++;
        }
    }

    float hitPercentage = (hitCount / static_cast<float>(totalTraces)) * 100.f;
    globals::game::ConsoleLog(std::to_string(hitPercentage) + " | " + std::to_string(hitCount), globals::config::secondaryColor);
    return hitPercentage;
}

void c_aimbot::on_aimbot_fire(c_base_player* target, float time, float hitChance, e_hitboxes targetHitbox, c_base_combat_weapon* weapon) {
    globals::game::user::ShotCount++;

    globals::ShotData newShot;
    player_info_t targetInfo;

    interfaces::m_engine->get_player_info(target->get_index(), &targetInfo);

    newShot.shotIndex = globals::game::user::ShotCount;
	newShot.attackerIndex = globals::m_local->get_info().m_user_id;
	newShot.targetIndex = targetInfo.m_user_id;
	newShot.time = time;
	newShot.hitChance = hitChance;
	newShot.targetHitbox = targetHitbox;
	newShot.weapon = weapon;    
	globals::game::user::m_ShotData.push_back(newShot);
}

bool c_aimbot::fireWeapon(int weaponType) {
    if (weaponType != WEAPON_TYPE_PISTOL)
    {
        if (globals::m_cur_cmd->m_buttons.has(IN_ATTACK))
            globals::m_cur_cmd->m_buttons.remove(IN_ATTACK);

        globals::m_cur_cmd->m_buttons.add(IN_ATTACK);
        globals::game::user::lastShotTime = interfaces::m_global_vars->m_cur_time;
        return true;
    }
    else
    {
        if (!globals::game::user::last_shot)
        {
            globals::m_cur_cmd->m_buttons.add(IN_ATTACK);
            globals::game::user::last_shot = true;
			globals::game::user::lastShotTime = interfaces::m_global_vars->m_cur_time;
            return true;
        }
        else
        {
            globals::m_cur_cmd->m_buttons.remove(IN_ATTACK);
            globals::game::user::last_shot = false;
            return false;
        }
    }
}

void c_aimbot::run()
{
    if (!globals::config::aimbotEnabled || !globals::m_local || !globals::m_local->is_alive())
        return;

    if (!globals::config::aimbotAutoShoot && !globals::config::silentAimEnabled && !globals::m_cur_cmd->m_buttons.has(IN_ATTACK))
        return;

    c_base_combat_weapon* activeWeapon;

    activeWeapon = globals::m_local->get_active_weapon();

    globals::game::user::can_shoot = (activeWeapon->get_next_primary_attack() <= interfaces::m_global_vars->m_cur_time);

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

    c_cs_player* bestTarget = nullptr;
    float bestMetric = FLT_MAX;
    vec3_t CurrentAimPunch{};
    vec3_t bestTargetAngle{};
	e_hitboxes bestHitbox{};

    for (const auto& data : gameEntityHandler->EntityStorage)
    {
        if (!data.entity || !data.entity->get_base_entity()->is_player())
            continue;

        c_cs_player* player;

        player = reinterpret_cast<c_cs_player*>(data.entity);

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
                int hitboxID = globals::config::hitboxTable[j].ref[k];
                hitbox = hitboxSet->get_hitbox(hitboxID);

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
                default:
                    break;
                } 

                if (metric < bestMetric)
                {
                    bestMetric = metric;
                    bestTargetAngle = angleToTarget;
                    CurrentAimPunch = aimPunch;
                    bestTarget = player;
                    bestHitbox = static_cast<e_hitboxes>(hitboxID);
                }
            }
        }
    }

    if (bestTarget)
    {
        autostop->run(activeWeapon);
        qangle_t finalAngles = bestTargetAngle.angle();
        finalAngles = finalAngles - (CurrentAimPunch.angle() + CurrentAimPunch.angle());

        if (globals::config::silentAimEnabled)
        {
            globals::m_cur_cmd->m_view_angles = finalAngles;
        }
        else
        {
            interfaces::m_engine->set_view_angles(finalAngles);
            globals::m_cur_cmd->m_view_angles = finalAngles;
        }

        if (weaponType == WEAPON_TYPE_SNIPER)
        {
            float timeSinceScope = interfaces::m_global_vars->m_cur_time - globals::game::user::lastScopeTime;

            bool shouldScope = globals::config::aimbotAutoscope && !globals::m_local->is_scoped();
            if (shouldScope && !globals::m_cur_cmd->m_buttons.has(IN_ZOOM))
            {
                if (globals::game::user::lastScopeTime == 0.f || timeSinceScope >= 0.2f)
                {
                    globals::m_cur_cmd->m_buttons.add(IN_ZOOM);
                }
            }

            bool isScoped = globals::m_local->is_scoped();

            if (isScoped && globals::game::user::lastScopeTime == 0.f)
            {
                globals::game::user::lastScopeTime = interfaces::m_global_vars->m_cur_time;
            }
            else if (!isScoped && globals::game::user::lastScopeTime != 0.f)
            {
                if (timeSinceScope >= 0.2f)
                {
                    globals::game::user::lastScopeTime = 0.f;
                }
            }
        }

        if (globals::config::aimbotAutoShoot && globals::game::user::can_shoot)
        {
            float currentHitchance = CalculateHitChance(activeWeapon, bestTarget, finalAngles);
            if (currentHitchance >= globals::config::aimbotHitchance)
            {
                if (fireWeapon(weaponType))
                {
					on_aimbot_fire(bestTarget, interfaces::m_global_vars->m_cur_time, currentHitchance, bestHitbox, activeWeapon);

                    bool status = interfaces::m_global_vars->m_cur_time >= activeWeapon->get_next_primary_attack();
                    globals::game::user::can_shoot = status;
                    globals::game::user::TryingToShoot = !status;
                }
                else
                {
					globals::game::user::can_shoot = true;
					globals::game::user::TryingToShoot = false;
                }
            }
        }
    }
    else
    {
        globals::game::user::TryingToShoot = false;
    }
}
