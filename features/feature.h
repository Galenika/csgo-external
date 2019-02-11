#pragma once
#include "../sdk/sdk.h"

class feature {
private:
	const char *name;

public:
	bool enabled = false;

	feature(const char *name) {
		this->name = name;
	}

	virtual auto tick() -> void = 0;
};