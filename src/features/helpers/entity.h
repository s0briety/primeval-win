#pragma once
#include "../../globals.h"

class EntityHandler : public c_singleton<EntityHandler> {
public:
	std::vector<globals::EntityData> EntityStorage;
	bool process();
};
#define gameEntityHandler EntityHandler::instance()