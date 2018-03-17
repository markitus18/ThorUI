#ifndef __LOG__
#define __LOG__

#ifdef THORUI_EXPORTS
#define THORUI_API __declspec(dllexport) 
#else
#define THORUI_API __declspec(dllimport) 
#endif

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__)
THORUI_API void log(const char file[], int line, const char* format, ...);

#endif // !__LOG__
