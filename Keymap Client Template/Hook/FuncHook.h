#pragma once

class FuncHook {
public:
	virtual bool Initialize() = 0;
};

// include hooks here
#include "Hooks/KeymapHook.h"

void InitHooks() {
	// initialize hooks here
	static FuncHook* hooks[] = {
		// include hooks here
        &KeymapHook::Get()
	};

    for (std::size_t i = 0; i < std::size(hooks); ++i)
    {
        if (not hooks[i]->Initialize())
        {
            //error handling
        }
    }
}