//
//
//  @ Project : SDMG
//  @ File Name : FileManager.h
//  @ Date : 27-11-2014
//  @ Author : 42IN13SAd
//
//

#pragma once

#include <string>
#include <vector>
#include <json.hpp>

namespace sdmg {
	namespace engine {
		namespace util {
			class FileManager {
			public:
				nlohmann::json loadFileContentsAsJson(const std::string &relativePath);
				void saveJsonContentToFile(const std::string& relativePath, const nlohmann::json& content);
				void createFolder(const std::string& fullFolderPath);
				const std::vector<std::string> getFiles(std::string path);
				const std::vector<std::string> getFolders(std::string path);
				
				static FileManager& getInstance() {
					static FileManager _instance;
					return _instance;
				}

			protected:
				FileManager() { }
			private:
				enum Type
				{
					Files,
					Folders
				};

				const std::vector<std::string> getList(std::string path, Type type);
			};
		}
	}
}