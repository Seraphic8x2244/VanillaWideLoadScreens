# VanillaWideLoadScreens

**WideLoadScreens** enables widescreen loading screens in the original 32-bit World of Warcraft 1.12.1 client without permanently modifying `WoW.exe`.

The DLL applies the same five-byte runtime patch as the working widescreen executable edit:

- Patch RVA: `0x00006AC4`
- Stock bytes: `E8 A7 42 01 00`
- Patched bytes: `D9 E8 90 90 90`

Before writing anything, the DLL checks that the expected stock bytes are present. If the executable does not match, it leaves the process untouched. If the client is already patched, it also does nothing.

## Requirements

- World of Warcraft 1.12.1, 32-bit
- A DLL loader such as VanillaFixes
- `ClassicAPI.dll` is still required for loading-screen textures that exceed the stock texture-size limitations used by this setup, including the 1920×1080 widescreen assets

The client still has a practical loading-screen texture size limit of roughly 2 MB in this setup, so 1080p assets work while very large 4K loading screens do not.

## VanillaFixes installation

Download:

```text
WideLoadScreens.dll
```

Place it alongside the WoW client and add this line to VanillaFixes' `dlls.txt`:

```text
WideLoadScreens.dll
```

This repository contains only the executable runtime patch. Widescreen loading-screen artwork/MPQ files are separate.

## Building

GitHub Actions builds the release as **32-bit x86** with MSVC.

For a local build, open an x86 Visual Studio Native Tools command prompt and run:

```bat
mkdir dist
cl /nologo /O2 /W4 /LD /MT /GS- src\WideLoadScreens.c /link /OUT:dist\WideLoadScreens.dll
```

The expected output is:

```text
dist/WideLoadScreens.dll
```
