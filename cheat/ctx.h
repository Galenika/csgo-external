#pragma once
#include <windows.h>
#include <vector>
#include "../sdk/entity_base.h"

class cheat_ctx {
public:
	entity_base *local;
	std::vector<entity_base*> entities;
};

extern cheat_ctx ctx;