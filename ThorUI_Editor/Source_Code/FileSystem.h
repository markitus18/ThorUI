#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <vector>
#include <string>

#include <Windows.h>
#include <ShObjIdl.h>
#include <comdef.h>

#include "SDL2-2.0.6\include\SDL.h"

namespace FileSystem
{
	std::string GetRelativePath(const char* base, const char* path);
	std::vector<std::string> SplitFilePath(const char* path);

	void NormalizePath(std::string& path);

	//* Given 2 split paths, returns the last index where the 2 paths are the same
	//* Return -1 if the paths have no common folders
	int GetLastEqualFolder(std::vector<std::string> path1, std::vector<std::string> path2);

	std::string OpenFileDialog();
}

#endif
