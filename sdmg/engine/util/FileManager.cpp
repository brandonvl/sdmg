//
//
//  @ Project : SDMG
//  @ File Name : FileManager.h
//  @ Date : 27-11-2014
//  @ Author : 42IN13SAd
//
//

#include "FileManager.h"

#include <array>
#include <random>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace sdmg {
	namespace engine {
		namespace util {

			const std::vector<std::string> FileManager::getFiles(std::string path)
			{
				return getList(path, Type::Files);
			}

			const std::vector<std::string> FileManager::getFolders(std::string path)
			{
				return getList(path, Type::Folders);
			}

			const std::vector<std::string> FileManager::getList(std::string path, Type type)
			{
				std::vector<std::string> list;

				path = path + "*.*";
				WIN32_FIND_DATA fd;
				HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
				if (hFind != INVALID_HANDLE_VALUE)
				{
					do {
						if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && type == Type::Folders && fd.cFileName[0] != '.')
							list.push_back(fd.cFileName);
						else if (type == Type::Files && fd.cFileName[0] != '.')
							list.push_back(fd.cFileName);
					} while (::FindNextFile(hFind, &fd));
					::FindClose(hFind);
				}

				return list;
			}

			const bool FileManager::dirExists(std::string path) {
				struct stat info;

				path = "C:/Users/Wouter/Documents/GitHub/sdmg/sdmg/" + path;
				if (stat(path.c_str(), &info) != 0)
					return false;
				else if (info.st_mode & S_IFDIR)  // S_ISDIR() doesn't exist on my windows 
					return true;
				else
					return false;
			}
		}
	}
}