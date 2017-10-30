#include "FileSystem.h"

namespace FileSystem
{
	std::string GetRelativePath(const char* base, const char* path)
	{
		std::string ret = "";

		std::vector<std::string> base_split = SplitFilePath(base);
		std::vector<std::string> path_split = SplitFilePath(path);

		int lastEqual = GetLastEqualFolder(base_split, path_split);

		for (int i = lastEqual + 1; i < base_split.size(); i++)
		{
			ret.append("../");
		}
		for (int i = lastEqual + 1; i < path_split.size(); i++)
		{
			ret.append(path_split[i]);
		}
		return ret;
	}

	std::vector<std::string> SplitFilePath(const char* path)
	{
		std::vector<std::string> ret;
		std::string str(path);
		NormalizePath(str);

		bool split = true;
		while (split == true)
		{
			split = false;
			size_t separator = str.find_first_of("/");
			if (separator != std::string::npos)
			{
				split = true;
				ret.push_back(str.substr(0, separator + 1));
				str = str.substr(separator + 1);
			}
		}
		if (str != "")
			ret.push_back(str);
		return ret;
	}

	void NormalizePath(std::string& path)
	{
		for (int i = 0; i < path.length(); ++i)
		{
			if (path[i] == '\\')
			{
				path[i] = '/';
			}
		}
	}

	int GetLastEqualFolder(std::vector<std::string> path1, std::vector<std::string> path2)
	{
		for (int i = 0; i < path1.size() && i < path2.size(); i++)
		{
			if (path1[i] != path2[i])
				return i - 1;
		}
		if ((int)path1.size() < (int)path2.size())
			return path1.size() - 1;
		return path2.size() - 1;
		//return (path1.size() < path2.size ? path1.size() : path2.size());
	}
}