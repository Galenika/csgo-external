#include "sdk/sdk.h"

memory mem;
cheat_ctx ctx;

auto main() -> int {
	SetConsoleTitleA("csgo external");

	if (!mem.attach("csgo.exe"))
		std::cout << "csgo not found." << std::endl;

	offsets::client = mem.get_module("client_panorama.dll", offsets::client_size);
	offsets::engine = mem.get_module("engine.dll", offsets::engine_size);
	offsets::client_state = mem.read<DWORD>(offsets::engine + offsets::signatures::dwClientState);

	while (!(GetAsyncKeyState(VK_F8) < 0)) {
		ctx.local = mem.read<entity_base*>(offsets::client + offsets::signatures::dwLocalPlayer);
		if (!ctx.local->alive())
			continue;

		ctx.entities.clear();
		for (auto i = 0; i < 64; i++) {
			auto entity = mem.read<entity_base*>(offsets::client + offsets::signatures::dwEntityList + (i - 1) * 0x10);
			if (!entity)
				continue;

			ctx.entities.push_back(entity);
		}

		for (auto feature : cheat::get_features()) {
			feature->tick();
		}

		Sleep(1);
	}

	cheat::get_features().clear();
	mem.close();
	return 0;
}