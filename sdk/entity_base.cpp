#include "entity_base.h"
#include "../cheat/ctx.h"

auto entity_base::health() -> int {
	return mem.read<int>(reinterpret_cast<DWORD>(this) + offsets::netvars::m_iHealth);
}

auto entity_base::team() -> int {
	return mem.read<int>(reinterpret_cast<DWORD>(this) + offsets::netvars::m_iTeamNum);
}

auto entity_base::in_game() -> bool {
	return mem.read<int>(offsets::client_state + offsets::signatures::dwClientState_State) == 6;
}

auto entity_base::alive() -> bool {
	return this->health() > 0 && mem.read<int8_t>(reinterpret_cast<DWORD>(this) + offsets::netvars::m_lifeState) == 0;
}

auto entity_base::dormant() -> bool {
	return mem.read<int>(reinterpret_cast<DWORD>(this) + offsets::signatures::m_bDormant);
}

auto entity_base::spotted() -> bool {
	return mem.read<int>(reinterpret_cast<DWORD>(this) + offsets::netvars::m_bSpotted);
}

auto entity_base::enemy() -> bool {
	return ctx.local->team() != this->team();
}

auto entity_base::velocity() -> vec3 {
	return mem.read<vec3>(reinterpret_cast<DWORD>(this) + offsets::netvars::m_vecVelocity);
}