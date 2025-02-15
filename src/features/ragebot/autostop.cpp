#include "../features.h"

void c_autostop::ApplyAutostop(float maxSpeed) {
    if (maxSpeed == 255.f)
        return;

    vec3_t currentMove = globals::m_cur_cmd->m_move;
    float currentMoveSpeed = currentMove.length();

    if (currentMoveSpeed > maxSpeed && currentMoveSpeed > 0.f) {
        float scale = maxSpeed / currentMoveSpeed;
        globals::m_cur_cmd->m_move *= scale;
    }
}

void c_autostop::run(c_base_combat_weapon* activeWeapon) {
    // Get current velocity and compute speed.
    vec3_t currentVelocity = globals::m_local->get_velocity();
    float currentSpeed = currentVelocity.length();

    // Get weapon inaccuracy and compute a dynamic factor.
    float inaccuracy = activeWeapon->get_inaccuracy();
    float dynamicFactor = std::max(0.5f, 1.f - inaccuracy * 0.5f);

    if (!globals::game::user::TryingToShoot && globals::config::autostopTable[1].isEnabled)
    {
        return;
    }

    // Otherwise, process the other autostop modes.
    for (int i = 0; i < 6; i++) {
        if (!globals::config::autostopTable[i].isEnabled)
            continue;

        float baseMaxSpeed = 255.f; // default: no autostop

        switch (globals::config::autostopTable[i].id) {
        case 1: // Early: strong autostop if moving fast; base: 30 units.
            if (currentSpeed > 5.0f)
                baseMaxSpeed = 30.f;
            break;
        case 2: // Between Shots: already handled above.
            break;
        case 3: // Aggressive: nearly full autostop; base: 10 units.
            baseMaxSpeed = 10.f;
            break;
        case 4: // Passive: slight reduction; base: 70 units.
            baseMaxSpeed = 70.f;
            break;
        case 5: // Smart: additional logic could be applied; base: 50 units.
            baseMaxSpeed = 50.f;
            break;
        case 6: // Jumpscout: special handling when in the air; base: 30 units.
            if (!globals::m_local->get_flags().has(FL_ONGROUND))
                baseMaxSpeed = 30.f;
            break;
        default:
            break;
        }

        // Apply dynamic factor to the base max speed.
        float finalMaxSpeed = baseMaxSpeed * dynamicFactor;
        ApplyAutostop(finalMaxSpeed);
    }
}

