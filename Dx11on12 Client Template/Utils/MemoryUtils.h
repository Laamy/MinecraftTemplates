#pragma once

// fake fields
#define OFFSET_FIELD(type, name, offset) \
    type name##() { \
        return *reinterpret_cast<type*>(reinterpret_cast<char*>(this) + offset); \
    } \
    void name##(type new_##name) { \
        *reinterpret_cast<type*>(reinterpret_cast<char*>(this) + offset) = new_##name; \
    }

// util used to call functions by their void* address
template <typename R, typename... Args>
R CallFunc(void* func, Args... args) {
    return ((R(*)(Args...))func)(args...);
}

// util used to call functions by their uintptr_t address
template <typename R, typename... Args>
R CallFunc(uintptr_t func, Args... args) {
    return ((R(*)(Args...))func)(args...);
}

// minecraft base address
uintptr_t range_start = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");

// function used to hook game functions
bool HookFunction(void* pTarget, void* pDetour, void* pOriginal) {

    static bool initialized = false;

    if (!initialized) {
        initialized = true;

        if (MH_Initialize() != MH_OK) {
            return false;
        }
    }

    if (MH_CreateHook(pTarget, pDetour, (LPVOID*)pOriginal) != MH_OK) {
        return false;
    }

    if (MH_EnableHook(pTarget) != MH_OK) {
        return false;
    }

    return true;

}

static void* findMultiLvlPtr(uintptr_t baseAddr, std::vector<unsigned int> offsets) {

    uintptr_t addr = baseAddr;

    for (int I = 0; I < offsets.size(); I++)
    {
        addr = *(uintptr_t*)(addr);

        if ((uintptr_t*)(addr) == nullptr)
        {
            return reinterpret_cast<void*>(addr);
        }

        addr += offsets[I];
    };

    return reinterpret_cast<void*>(addr);

};

#define INRANGE(x, a, b) (x >= a && x <= b)
#define GET_BYTE(x) ((GET_BITS(x[0]) << 4) | GET_BITS(x[1]))
#define GET_BITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? (x - '0') : 0))

static uintptr_t findSig(const char* sig) {

    const char* pattern = sig;
    uintptr_t first_match = 0;
    static MODULEINFO mi_mod_info;
    static bool init = false;
    if (!init)
    {
        init = true;
        GetModuleInformation(GetCurrentProcess(), reinterpret_cast<HMODULE>(range_start), &mi_mod_info, sizeof(MODULEINFO));
    }
    static const uintptr_t range_end = range_start + mi_mod_info.SizeOfImage;

    BYTE pat_byte = GET_BYTE(pattern);
    const char* old_pat = pattern;

    for (uintptr_t p_cur = range_start; p_cur < range_end; p_cur++) // eevrything we use is over 0x100000 so we can just skip that part of memory
    {
        if (!*pattern)
        {
            return first_match;
        }

        while (*(PBYTE)pattern == ' ')
        {
            pattern++;
        }

        if (!*pattern)
        {
            return first_match;
        }

        if (old_pat != pattern)
        {
            old_pat = pattern;
            if (*(PBYTE)pattern != '\?') {
                pat_byte = GET_BYTE(pattern);
            }
        }

        if (*(PBYTE)pattern == '\?' || *(BYTE*)p_cur == pat_byte)
        {
            if (!first_match)
            {
                first_match = p_cur;
            }

            if (!pattern[2])
            {
                return first_match;
            }

            pattern += 2;
        }
        else
        {
            pattern = sig;
            first_match = 0;
        }
    }

    return 0;

}

static uintptr_t findOffset(const char* sig, const char* sidId) {

    uintptr_t sigResult = findSig(sig);

    if (sigResult == 0) {

        std::stringstream ss;
        ss << "[" << sidId << "] Failed to find signiture";
        std::cout << ss.str().c_str() << std::endl;

        return 0;
    }
    else {
        std::cout << "[" << sidId << ", " << std::hex << sigResult << "] Successfully created hook" << std::endl;
    }

    return sigResult;

}