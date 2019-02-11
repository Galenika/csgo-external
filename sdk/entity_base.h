#pragma once
#include "../util/memory.h"
#include "../util/offsets.h"
#include "math.h"

class entity_base {
public:
	auto health() -> int;
	auto team() -> int;
	auto in_game() -> bool;
	auto alive() -> bool;
	auto dormant() -> bool;
	auto spotted() -> bool;
	auto enemy() -> bool;
	auto velocity() -> vec3;
};