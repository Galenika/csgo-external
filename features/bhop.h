#pragma once
#include "feature.h"


class bhop : public feature {
public:
	bhop() : feature("Bunny Hop") { enabled = true; }

	auto tick() -> void {
		if (!(GetAsyncKeyState(VK_SPACE) < 0))
			return;

		auto velocity = ctx.local->velocity();
		if (velocity.x != 0 && velocity.y != 0) {
			// force jump.
			mem.write<int>(6, offsets::client + offsets::signatures::dwForceJump);
		}
	}
};