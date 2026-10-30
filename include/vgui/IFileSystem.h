#pragma once
#include "VGui.h"

namespace vgui
{
    class IFileSystem
    {
    public:
        virtual void* Open(const char* pFileName, const char* pOptions) = 0;
        virtual void Close(void* file) = 0;
        virtual int Read(void* pOutput, int size, void* file) = 0;
        virtual int Write(void const* pInput, int size, void* file) = 0;
        virtual int Size(const char* pFileName) = 0;
        virtual bool FileExists(const char* pFileName) = 0;
        virtual void Seek(void* file, int pos, int seekType) = 0;
        virtual int Tell(void* file) = 0;
    };
    
    #define VFILESYSTEM_INTERFACE_VERSION "VFileSystem009"
}
