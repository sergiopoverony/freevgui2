cmake_minimum_required(VERSION 3.10)
project(vgui2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(vgui2 SHARED
    src/dllmain.cpp
    src/interfaces/Panel.cpp
    src/interfaces/Surface.cpp
    src/interfaces/VGui.cpp
    src/interfaces/VPanel.cpp
    src/interfaces/SchemeManager.cpp
    src/interfaces/Controls.cpp
    src/interfaces/PanelFactory.cpp
    src/interfaces/CPanel.cpp
    src/interfaces/CFrame.cpp
    src/interfaces/CButton.cpp
    src/interfaces/CLabel.cpp
    src/interfaces/CTextEntry.cpp
    src/interfaces/System.cpp
    src/interfaces/Input.cpp
    src/interfaces/IVGui.cpp
    src/core/KeyValues.cpp
    src/core/Localize.cpp
    src/core/CUtlVector.cpp
    src/core/UtlVector_Int.cpp
    src/core/UtlVector_Element.cpp
)

# Linker map (vgui2.map) lets us translate RVAs from vgui2_debug.log into function names
set_target_properties(vgui2 PROPERTIES LINK_FLAGS "/MAP:vgui2.map /DEBUG /OPT:NOICF")

target_include_directories(vgui2 PRIVATE include)
target_compile_definitions(vgui2 PRIVATE WIN32 _WINDOWS _USRDLL vgui2_EXPORTS)
target_link_libraries(vgui2 PRIVATE kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32 winmm)

add_subdirectory(test)
