#pragma once
#include "../features.h"

class c_logging : public c_singleton<c_logging> {
public:
    auto ConsoleLog(std::string_view msg, col_t color = globals::config::secondaryColor) -> void;
	void aimbot_log(int ShotIndex);
private:
	std::string hitbox_to_string(e_hitboxes hitbox);
};
#define logging c_logging::instance()
