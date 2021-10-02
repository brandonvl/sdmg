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
#include <filesystem>
#include <fstream>
#include <iostream>

namespace sdmg {
	namespace engine {
		namespace util {

			nlohmann::json FileManager::loadFileContentsAsJson(const std::string& relativePath)
			{
				try
				{ 
					std::ifstream ifs(relativePath);
					auto json = nlohmann::json::parse(ifs);
					ifs.close();
					return json;
				}
				catch (...)
				{
					std::cout << "Unable to load " + relativePath + ". Unlooky" << std::endl;
					throw;
				}				
			}

			void FileManager::saveJsonContentToFile(const std::string& relativePath, const nlohmann::json& content)
			{
				try
				{
					// Overwrites file by default
					std::ofstream ofs(relativePath);
					ofs << content;
					ofs.close();
				}
				catch (...)
				{
					std::cout << "Unable to save " + relativePath + ". Unlooky" << std::endl;
					throw;
				}
			}

			void FileManager::createFolder(const std::string& fullFolderPath)
			{
				std::filesystem::create_directories(fullFolderPath);
			}

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

				for (auto& entry : std::filesystem::directory_iterator(path))
				{
					if (type == Type::Folders)
					{
						if (std::filesystem::is_directory(entry))
						{
							list.push_back(entry.path().stem().string());
						}
					}
					else if (type == Type::Files)
					{
						if (std::filesystem::is_regular_file(entry))
						{
							list.push_back(entry.path().stem().string());
						}
					}
				}

				/*WIN32_FIND_DATA fd;
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
				}*/

				return list;
			}
		}
	}
}