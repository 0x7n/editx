#pragma once

#include <vector>
#include <string>

class FileSystem {
public:
	struct FileInfo {
		std::string name;
		bool isDirectory;
		std::string path;
	};

	FileSystem() = delete;

	static std::vector<FileInfo> listFiles(const std::string& dir);
	static std::string readFile(const std::string& path);
	static void writeFile(const std::string& path, const std::string& content);
};