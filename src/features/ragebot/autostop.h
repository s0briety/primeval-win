#pragma once
#include "../../globals.h"

class c_autostop : public c_singleton<c_autostop> {
public:
	void run(c_base_combat_weapon* activeWeapon);
private:
	void ApplyAutostop(float maxSpeed);
};
#define autostop c_autostop::instance()