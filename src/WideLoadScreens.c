#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if !defined(_M_IX86)
#error WideLoadScreens must be built for 32-bit x86.
#endif

#define PATCH_RVA 0x00006AC4u
#define PATCH_SIZE 5u

static BOOL bytes_equal(const BYTE *a, const BYTE *b, SIZE_T count)
{
    SIZE_T i;

    for (i = 0; i < count; ++i) {
        if (a[i] != b[i]) {
            return FALSE;
        }
    }

    return TRUE;
}

static void bytes_copy(BYTE *destination, const BYTE *source, SIZE_T count)
{
    SIZE_T i;

    for (i = 0; i < count; ++i) {
        destination[i] = source[i];
    }
}

static void apply_widescreen_loadscreen_patch(void)
{
    static const BYTE expected[PATCH_SIZE] = {
        0xE8, 0xA7, 0x42, 0x01, 0x00
    };
    static const BYTE patched[PATCH_SIZE] = {
        0xD9, 0xE8, 0x90, 0x90, 0x90
    };

    HMODULE executable;
    BYTE *patch_site;
    DWORD old_protection;
    DWORD ignored;

    executable = GetModuleHandleA(NULL);
    if (executable == NULL) {
        return;
    }

    patch_site = (BYTE *)executable + PATCH_RVA;

    /* Already patched: leave it alone. */
    if (bytes_equal(patch_site, patched, PATCH_SIZE)) {
        return;
    }

    /* Refuse to alter an unexpected executable build. */
    if (!bytes_equal(patch_site, expected, PATCH_SIZE)) {
        return;
    }

    if (!VirtualProtect(
            patch_site,
            PATCH_SIZE,
            PAGE_EXECUTE_READWRITE,
            &old_protection)) {
        return;
    }

    bytes_copy(patch_site, patched, PATCH_SIZE);
    FlushInstructionCache(GetCurrentProcess(), patch_site, PATCH_SIZE);

    VirtualProtect(
        patch_site,
        PATCH_SIZE,
        old_protection,
        &ignored);
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    (void)instance;
    (void)reserved;

    if (reason == DLL_PROCESS_ATTACH) {
        apply_widescreen_loadscreen_patch();
    }

    return TRUE;
}
