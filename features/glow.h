#pragma once
#include "feature.h"


class glow : public feature {
public:
	glow() : feature("Glow") { enabled = true; }

	auto tick() -> void {
		auto glow_manager = mem.read<int>(offsets::client + offsets::signatures::dwGlowObjectManager);

		for (auto entity : ctx.entities) {
			if (!entity->alive() ||
				!entity->enemy() ||
				entity->dormant())
				continue;

			auto entity_index = mem.read<int>(reinterpret_cast<DWORD>(entity) + offsets::netvars::m_iGlowIndex);
			auto glow_struct = mem.read<game_glow>(glow_manager + entity_index * 0x38 + 0x4);

			glow_struct.r = 242.0f / 255.0f;
			glow_struct.g = 218.0f / 255.0f;
			glow_struct.b = 36.0f / 255.0f;
			glow_struct.a = 130.0f / 255.0f;
			glow_struct.render_when_occluded = true;
			glow_struct.render_when_unoccluded = false;

			auto glow_update = mem.find_pattern(offsets::client, offsets::client_size, "\x74\x07\x8B\xCB\xE8\x00\x00\x00\x00\x83\xC7\x10", "xxxxx????xxx") - offsets::client;

			mem.write<unsigned char>(0xEB, offsets::client + glow_update);
			mem.write<game_glow>(glow_struct, glow_manager + entity_index * 0x38 + 0x4);
		}
	}

private:
	struct game_glow {
		float r, g, b, a;
		unsigned char unknown[16];
		bool render_when_occluded;
		bool render_when_unoccluded;
		bool full_bloom;
		int glow_style;
		unsigned char unknown1[10];
	};
};