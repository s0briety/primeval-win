#include "../../globals.h"

class c_esp : public c_singleton<c_esp> {
public:
	void CallEsp();
};
#define player_esp c_esp::instance()