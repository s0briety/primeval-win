#pragma once
#include "../../globals.h"

class c_aimbot : public c_singleton<c_aimbot> {
public:
    void run();
private:
    float CalculateDistance(const vec3_t& target, const vec3_t& me);
    void CalculateSpread(float inaccuracy, float spread, float& spreadX, float& spreadY);
    vec3_t PredictTargetPosition(const vec3_t& targetPos, const vec3_t& targetVel);
	vec3_t TransformHitboxPoint(const mstudiobbox_t* hitbox, const matrix3x4_t* bones);
	std::vector<vec3_t> c_aimbot::GenerateMultipoints(const mstudiobbox_t* hitbox, int bone, const matrix3x4_t& boneMatrix);
    bool TraceToTarget(const vec3_t& eyePos, const vec3_t& position, const vec3_t& spreadDirection, c_base_player* target);
    float CalculateHitChance(c_base_combat_weapon* weapon, c_base_player* target, qangle_t AngleToTarget);
    bool fireWeapon(int weaponType);
    void on_aimbot_fire(c_base_player* target, float time, float hitChance, e_hitboxes targetHitbox, c_base_combat_weapon* weapon);
};
#define aimbot c_aimbot::instance()