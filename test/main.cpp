#include <windows.h>
#include <iostream>

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

int main()
{
    HMODULE hVgui = LoadLibraryA("vgui2.dll");
    if (!hVgui)
    {
        std::cout << "Failed to load vgui2.dll! Error: " << GetLastError() << std::endl;
        return 1;
    }
    
    CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(hVgui, "CreateInterface");
    if (!CreateInterface)
    {
        std::cout << "Failed to get CreateInterface!" << std::endl;
        FreeLibrary(hVgui);
        return 1;
    }
    
    void* pSurface = CreateInterface("VGUI_Surface026", nullptr);
    void* pPanel = CreateInterface("VGUI_Panel007", nullptr);
    void* pInput = CreateInterface("VGUI_Input004", nullptr);
    void* pLocalize = CreateInterface("VGUI_Localize003", nullptr);
    void* pKeyValues = CreateInterface("KeyValues003", nullptr);
    void* pFileSystem = CreateInterface("VFileSystem009", nullptr);
    void* pSchemeManager = CreateInterface("VGUI_Scheme010", nullptr);
    
    std::cout << "=== VGUI2 DLL Test ===" << std::endl;
    std::cout << "Surface:      " << (pSurface ? "OK" : "FAIL") << std::endl;
    std::cout << "Panel:        " << (pPanel ? "OK" : "FAIL") << std::endl;
    std::cout << "Input:        " << (pInput ? "OK" : "FAIL") << std::endl;
    std::cout << "Localize:     " << (pLocalize ? "OK" : "FAIL") << std::endl;
    std::cout << "KeyValues:    " << (pKeyValues ? "OK" : "FAIL") << std::endl;
    std::cout << "FileSystem:   " << (pFileSystem ? "OK" : "FAIL") << std::endl;
    std::cout << "SchemeManager:" << (pSchemeManager ? "OK" : "FAIL") << std::endl;
    
    FreeLibrary(hVgui);
    
    std::cout << std::endl << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}
