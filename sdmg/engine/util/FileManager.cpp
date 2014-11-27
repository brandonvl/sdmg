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

namespace sdmg {
	namespace engine {
		namespace util {

			std::vector<std::string*> *FileManager::getFiles(std::string path)
			{
				return getList(path, Type::Files);
			}

			std::vector<std::string*> *FileManager::getFolders(std::string path)
			{
				return getList(path, Type::Folders);
			}

			std::vector<std::string*> *FileManager::getList(std::string path, Type type)
			{
				std::vector<std::string*> *list = new std::vector<std::string*>();

				path = path + "*.*";
				WIN32_FIND_DATA fd;
				HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
				if (hFind != INVALID_HANDLE_VALUE)
				{
					do {
						if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && type == Type::Folders && fd.cFileName[0] != '.')
							list->push_back(new std::string(fd.cFileName));
						else if (type == Type::Files && fd.cFileName[0] != '.')
							list->push_back(new std::string(fd.cFileName));
					} while (::FindNextFile(hFind, &fd));
					::FindClose(hFind);
				}

				return list;
			}
		}
	}
}