#include <algorithm>
#include <string>
#include <sstream>
#include "logging.h"

auto c_logging::ConsoleLog(std::string_view msg, col_t color) -> void
{

    std::string output = "[PRIMEVAL] | ";
    output.append(msg);
    output.append("\n");

    interfaces::m_cvar_system->console_print(color, output.data());
}

std::string c_logging::hitbox_to_string(e_hitboxes hitbox) {
    std::string hitbox_str;
    switch (hitbox) {
    case HITBOX_HEAD: hitbox_str = "HEAD"; break;
    case HITBOX_NECK: hitbox_str = "NECK"; break;
    case HITBOX_PELVIS: hitbox_str = "PELVIS"; break;
    case HITBOX_STOMACH: hitbox_str = "STOMACH"; break;
    case HITBOX_LOWER_CHEST: hitbox_str = "LOWER CHEST"; break;
    case HITBOX_CHEST: hitbox_str = "CHEST"; break;
    case HITBOX_UPPER_CHEST: hitbox_str = "UPPER CHEST"; break;
    case HITBOX_RIGHT_THIGH: hitbox_str = "RIGHT THIGH"; break;
    case HITBOX_LEFT_THIGH: hitbox_str = "LEFT THIGH"; break;
    case HITBOX_RIGHT_CALF: hitbox_str = "RIGHT CALF"; break;
    case HITBOX_LEFT_CALF: hitbox_str = "LEFT CALF"; break;
    case HITBOX_RIGHT_FOOT: hitbox_str = "RIGHT FOOT"; break;
    case HITBOX_LEFT_FOOT: hitbox_str = "LEFT FOOT"; break;
    case HITBOX_RIGHT_HAND: hitbox_str = "RIGHT HAND"; break;
    case HITBOX_LEFT_HAND: hitbox_str = "LEFT HAND"; break;
    case HITBOX_RIGHT_UPPER_ARM: hitbox_str = "RIGHT UPPER_ARM"; break;
    case HITBOX_RIGHT_FOREARM: hitbox_str = "RIGHT FOREARM"; break;
    case HITBOX_LEFT_UPPER_ARM: hitbox_str = "LEFT UPPER_ARM"; break;
    case HITBOX_LEFT_FOREARM: hitbox_str = "LEFT FOREARM"; break;
    default: hitbox_str = "UNKNOWN"; break;
    }

    return hitbox_str;
}

void c_logging::aimbot_log(int ShotIndex) {
    float curtime = interfaces::m_global_vars->m_cur_time;

    auto& shot = globals::game::user::m_ShotData[ShotIndex];

    if (shot.processed)
        return;

    shot.processed = true;

    std::string result;
    if (shot.kill) {
        result = "Eliminated";
    }
    else if (!shot.kill && shot.hitgroup >= 0) {
        result = "Hit";
    }
    else {
        result = "Missed";
    }

    std::ostringstream oss;
    int id = interfaces::m_engine->get_player_for_user_id(shot.targetIndex);
    player_info_t info;
    interfaces::m_engine->get_player_info(id, &info);
    const char* player_name = info.m_name ? info.m_name : "Unknown";

    std::string hitgroupStr;
    std::string target_hitbox = hitbox_to_string(shot.targetHitbox);

    switch (shot.hitgroup) {
    case 0:
        hitgroupStr = target_hitbox;
        break;
    case 1:
        hitgroupStr = "Head";
        break;
    case 2:
        hitgroupStr = "Chest";
        break;
    case 3:
        hitgroupStr = "Stomach";
        break;
    case 4:
        hitgroupStr = "Left Arm";
        break;
    case 5:
        hitgroupStr = "Right Arm";
        break;
    case 6:
        hitgroupStr = "Left Leg";
        break;
    case 7:
        hitgroupStr = "Right Leg";
        break;
    default:
        hitgroupStr = target_hitbox;
        break;
    }

    if (result == "Eliminated") {
        oss << result << " " << player_name << " with a shot to the " << hitgroupStr
            << " [HC: " << shot.hitChance << "% | TARGET: " << target_hitbox << "]";
    }
    else if (result == "Hit") {
        oss << result << " " << player_name << " in the " << hitgroupStr << " for " << shot.damage
            << " [HC: " << shot.hitChance << "% | TARGET: " << target_hitbox << "]";
    }
    else if (result == "Missed") {
        oss << result << " " << player_name << "'s " << target_hitbox << " due to " << shot.message
            << " [HC: " << shot.hitChance << "% | TARGET: " << target_hitbox << "]";
    }
    else {
        return;
    }

    std::string msg = oss.str();
    if (msg.empty())
        return;

    ConsoleLog(msg, globals::config::secondaryColor);
}

