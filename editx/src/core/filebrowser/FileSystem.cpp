#include "FileSystem.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace fs = std::filesystem;

std::vector<FileSystem::FileInfo> FileSystem::listFiles(const std::string& directory) {
    std::vector<FileInfo> items;
    for (const auto& entry : fs::directory_iterator(directory)) {
        FileInfo info;
        info.name = entry.path().filename().string();
        info.isDirectory = entry.is_directory();
        info.path = entry.path().string();
        items.push_back(info);
    }
    return items;
}

std::string FileSystem::readFile(const std::string& path)
{
	std::ifstream file(path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

void FileSystem::writeFile(const std::string& path, const std::string& content) {
	std::ofstream file(path);
	file << content;
}