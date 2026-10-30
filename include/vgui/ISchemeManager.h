#pragma once
#include "VGui.h"

namespace vgui
{
    // VGUI_Scheme009: 14 slots, argument counts verified against the original
    // (original/vtable_signatures.txt). Names follow the Source SDK; slots 11..13 are
    // not identified yet.
    class ISchemeManager
    {
    public:
        virtual ~ISchemeManager() {}                                                    // 0
        virtual HScheme LoadSchemeFromFile(const char* fileName, const char* tag) = 0;  // 1
        virtual void ReloadSchemes() = 0;                                               // 2
        virtual HScheme GetDefaultScheme() = 0;                                         // 3
        virtual HScheme GetScheme(const char* tag) = 0;                                 // 4
        virtual void* GetImage(const char* imageName, bool hardwareFiltered) = 0;       // 5
        virtual HTexture GetImageID(const char* imageName, bool hardwareFiltered) = 0;  // 6
        virtual void* GetIScheme(HScheme scheme) = 0;                                   // 7
        virtual void Shutdown(bool full) = 0;                                           // 8
        virtual int GetProportionalScaledValue(int normalizedValue) = 0;                // 9
        virtual int GetProportionalNormalizedValue(int scaledValue) = 0;                // 10
        virtual int Slot11() = 0;                                                       // 11
        virtual int Slot12(int a1) = 0;                                                 // 12
        virtual int Slot13(int a1) = 0;                                                 // 13
    };

    #define VGUI_SCHEMEMANAGER_INTERFACE_VERSION "VGUI_Scheme009"
}
