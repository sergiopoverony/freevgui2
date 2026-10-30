#include "vgui/IFileSystem.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace vgui
{
    class CFileSystem : public IFileSystem
    {
    public:
        CFileSystem() {}
        ~CFileSystem() {}
        
        void* Open(const char* pFileName, const char* pOptions) override
        {
            if (!pFileName || !pOptions) return nullptr;
            return fopen(pFileName, pOptions);
        }
        
        void Close(void* file) override
        {
            if (file) fclose((FILE*)file);
        }
        
        int Read(void* pOutput, int size, void* file) override
        {
            if (!pOutput || !file) return 0;
            return (int)fread(pOutput, 1, size, (FILE*)file);
        }
        
        int Write(void const* pInput, int size, void* file) override
        {
            if (!pInput || !file) return 0;
            return (int)fwrite(pInput, 1, size, (FILE*)file);
        }
        
        int Size(const char* pFileName) override
        {
            if (!pFileName) return 0;
            FILE* f = fopen(pFileName, "rb");
            if (!f) return 0;
            fseek(f, 0, SEEK_END);
            int size = (int)ftell(f);
            fclose(f);
            return size;
        }
        
        bool FileExists(const char* pFileName) override
        {
            if (!pFileName) return false;
            FILE* f = fopen(pFileName, "rb");
            if (f)
            {
                fclose(f);
                return true;
            }
            return false;
        }
        
        void Seek(void* file, int pos, int seekType) override
        {
            if (!file) return;
            fseek((FILE*)file, pos, seekType);
        }
        
        int Tell(void* file) override
        {
            if (!file) return 0;
            return (int)ftell((FILE*)file);
        }
    };
    
    static CFileSystem g_FileSystemInstance;
    
    void* CreateFileSystemReal()
    {
        return &g_FileSystemInstance;
    }
}
