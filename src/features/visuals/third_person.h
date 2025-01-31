#pragma once

#include "../../globals.h"

class c_thirdperson : public c_singleton<c_thirdperson> {
public:
	void EnterThirdPerson();
private:
	void ExitThirdPerson();
	float OriginalDistance = 0;
};
#define thirdperson c_thirdperson::instance()