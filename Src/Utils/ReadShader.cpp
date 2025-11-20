//
// Created by Jingren Bai on 25-7-15.
//
#define _CRT_SECURE_NO_WARNINGS

#include <regex>
#include <filesystem>
#include <unordered_set>
#include "ReadShader.h"
#include "log.cpp"

// 过滤掉 include 文件中多余的 #version 指令
static std::string stripExtraVersionDirectives(const std::string& src) {
	std::stringstream input(src);
	std::stringstream output;
	std::string line;
	while (std::getline(input, line)) {
		std::string trimmed = line;
		trimmed.erase(0, trimmed.find_first_not_of(" \t"));
		if (trimmed.rfind("#version", 0) == 0) {
			continue; // 跳过所有 #version 行
		}
		output << line << "\n";
	}
	return output.str();
}
// cache raw file contents
std::map<std::string, std::string> ReadShader::includeCache;
// cache processed shader sources (with includes expanded)
static std::map<std::string, std::string> processedCache;

// Helper: process shader recursively with visited set to prevent circular includes
static std::string processShaderInternal(const std::string& path, std::unordered_set<std::string>& visited);

ReadShader::ReadShader(const std::string& path) {
	shaderSource = processShader(path);
}

std::string ReadShader::readFile(const std::string& path) {
	if (includeCache.find(path) != includeCache.end()) {
		return includeCache[path]; // 已缓存，直接返回
	}

	std::ifstream file(path);
	if (!file.is_open()) {
		LOG_ERROR << "Cannot open shader include file: " << path;
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();

	// 添加到缓存
	includeCache[path] = content;
	return content;
}

std::string ReadShader::processShader(const std::string& path) {
	std::unordered_set<std::string> visited;
	return processShaderInternal(path, visited);
}

static std::string processShaderInternal(const std::string& path, std::unordered_set<std::string>& visited) {
	if (processedCache.find(path) != processedCache.end())
		return processedCache[path];

	if (visited.find(path) != visited.end()) {
		LOG_ERROR << "Circular include detected for: " << path;
		return "// [ERROR: Circular include: " + path + "]\n";
	}
	visited.insert(path);

	std::string source = ReadShader::readFile(path);
	if (source.empty())
		return "#version 430\n// [ERROR: File not found or empty]\n";

	std::stringstream input(source);
	std::stringstream output;
	std::string line;
	auto baseDir = std::filesystem::path(path).parent_path();

	std::regex includeRegex(R"(^\s*#include\s+\"([^\"]+)\"\s*$)");
	std::smatch match;

	while (std::getline(input, line)) {
		if (std::regex_match(line, match, includeRegex)) {
			std::string incFilename = match[1].str();
			std::filesystem::path includePath = baseDir / incFilename;
			includePath = includePath.lexically_normal();

			std::string incContent = processShaderInternal(includePath.string(), visited);
			incContent = stripExtraVersionDirectives(incContent);
			if (incContent.empty()) {
				LOG_ERROR << "Included file not found or empty: " << includePath.string();
				output << "// [WARNING: Included file not found: " << includePath.string() << "]\n";
			} else {
				output << "\n// [BEGIN include: " << incFilename << "]\n"
					   << incContent
					   << "\n// [END include: " << incFilename << "]\n";
			}
		} else {
			output << line << "\n";
		}
	}

	visited.erase(path);
	processedCache[path] = output.str();
	return output.str();
}



const char *ReadShader::getShader() const {
	return shaderSource.c_str();
}