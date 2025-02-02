#pragma once
#include "../../globals.h"

class c_aimbot : public c_singleton<c_aimbot> {
public:
    void run(c_user_cmd* cmd);
private:
    float CalculateDistance(const vec3_t& target, const vec3_t& me);
};
#define aimbot c_aimbot::instance()