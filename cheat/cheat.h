#pragma once
#include "../features/feature.h"
#include "../features/bhop.h"
#include "../features/glow.h"

namespace cheat {
	auto get_features() -> std::vector<feature*> {
		static std::vector<feature*> features = {
			new bhop,
			new glow
		};

		return features;
	}
}