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
#include <filesystem>
#include <json.hpp>

namespace sdmg {
	namespace engine {
		namespace util {

			class FileManager {
			public:
				void setExecutablePath(const std::string executablePath)
				{
					_executablePath = executablePath;
					_directoryPath = _executablePath.parent_path();
				}
				nlohmann::json loadFileContentsAsJson(const std::string &relativePath);
				void saveJsonContentToFile(const std::string& relativePath, const nlohmann::json& content);
				void createFolder(const std::string& fullFolderPath);
				bool fileExists(const std::string& relativePath);
				const std::vector<std::string> getFiles(std::string path);
				const std::vector<std::string> getFolders(std::string path);
				
				static FileManager& getInstance() {
					static FileManager s_Instance;
					return s_Instance;
				}

			protected:
				FileManager() { }
			private:
				enum Type
				{
					Files,
					Folders
				};

				std::filesystem::path combinePath(const std::string& relativePath);
				const std::vector<std::string> getList(std::string path, Type type);
				std::filesystem::path _executablePath;
				std::filesystem::path _directoryPath;
			};
		}
	}
}