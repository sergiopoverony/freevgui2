#pragma once

namespace vgui { extern void Log(const char* format, ...); }

// Logs only the first 16 calls from each call site: the log stays readable but
// still shows which interface methods the engine really calls (and in what order).
#define VGUI_TRACE(...) do { static int s_n = 0; if (s_n++ < 16) ::vgui::Log(__VA_ARGS__); } while (0)
