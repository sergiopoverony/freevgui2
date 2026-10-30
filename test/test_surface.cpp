#include <windows.h>
#include <cstdio>
#include "vgui/VGui.h"
#include "vgui/ISurface.h"

static HINSTANCE g_hDll = nullptr;
static vgui::ISurface* g_pSurface = nullptr;
static const wchar_t* kClassName = L"VguiTestWindow";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect(hwnd, &rc);

        if (g_pSurface)
        {
            g_pSurface->SetEmbeddedWindow(hwnd);

            g_pSurface->DrawSetColor(25, 35, 55, 255);
            g_pSurface->DrawFilledRect(0, 0, rc.right, rc.bottom);

            g_pSurface->DrawSetColor(90, 160, 255, 255);
            g_pSurface->DrawOutlinedRect(20, 20, rc.right - 20, rc.bottom - 20);

            g_pSurface->DrawSetColor(255, 200, 60, 255);
            g_pSurface->DrawLine(20, 90, rc.right - 20, 90);

            vgui::HFont font = g_pSurface->CreateFont();
            g_pSurface->SetFontGlyphSet(font, "Tahoma", 24, 700, 0, 0, 0);
            g_pSurface->DrawSetTextFont(font);
            g_pSurface->DrawSetTextColor(255, 255, 255, 255);
            g_pSurface->DrawSetTextPos(40, 45);
            g_pSurface->DrawPrintText(L"vgui2 reconstruction - OK", -1);

            g_pSurface->DrawSetTextPos(40, 110);
            g_pSurface->DrawSetTextColor(160, 200, 255, 255);
            g_pSurface->DrawPrintText(L"ISurface: GDI backend working", -1);
        }

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_ERASEBKGND:
        return 1;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    g_hDll = LoadLibraryA("C:\\vgui2_re\\build\\vgui2.dll");
    if (!g_hDll)
    {
        MessageBoxA(nullptr, "Failed to load vgui2.dll", "Error", MB_ICONERROR);
        return 1;
    }

    typedef void* (*CreateFn)(const char*, int*);
    CreateFn create = (CreateFn)GetProcAddress(g_hDll, "CreateInterface");
    if (!create)
    {
        MessageBoxA(nullptr, "CreateInterface not found", "Error", MB_ICONERROR);
        return 1;
    }

    int returnCode = 1;
    g_pSurface = (vgui::ISurface*)create("VGUI_Surface026", &returnCode);
    if (!g_pSurface)
    {
        char buf[256];
        sprintf(buf, "VGUI_Surface026 not found, code=%d", returnCode);
        MessageBoxA(nullptr, buf, "Error", MB_ICONERROR);
        return 1;
    }

    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW) };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = kClassName;
    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowExW(
        0, kClassName, L"vgui2 test", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 360,
        nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (g_pSurface) g_pSurface->Shutdown();
    FreeLibrary(g_hDll);
    return 0;
}
