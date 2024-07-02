#pragma once

void* __o__keypress;

void KeypressDetour(int key, bool held) {
	Game::Keymap[key] = held;

	std::cout << "key pressed " << key << " " << held << std::endl;

	return CallFunc<void, int, bool>(__o__keypress, key, held);
}

class KeymapHook : public FuncHook {
public:
	bool Initialize() override {
		uintptr_t keymapAddr = findOffset("48 83 EC 48 ? ? C1 4C 8D", "Keymap");

		if (not HookFunction((void*)keymapAddr, &KeypressDetour, &__o__keypress))
			return false;

		return true;
	}

	static KeymapHook& Get() {
		static KeymapHook single;
		return single;
	}
};