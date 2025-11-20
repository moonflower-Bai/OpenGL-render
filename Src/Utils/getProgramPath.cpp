//
// Created by Jingren Bai on 25-8-14.
//
#include "getProgramPath.h"

std::string getProgramPath() {
	char buffer[1024] = {0};

#ifdef _WIN32
	GetModuleFileNameA(NULL, buffer, sizeof(buffer));

	// 转成 std::string 好处理
	std::string fullPath(buffer);

	// 找最后一个反斜杠（路径分隔）
	size_t pos = fullPath.find_last_of("\\/");
	if (pos != std::string::npos) {
		return fullPath.substr(0, pos);  // 返回目录部分
	}

	return fullPath; // 不太可能走到这里
#elif __APPLE__
	uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        // 将相对路径转换为绝对路径
        char* resolved = realpath(buffer, NULL);
        if (resolved) {
            std::string result(resolved);
            free(resolved);
            return result;
        }
    }
#else // Linux
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
    if (len != -1) {
        buffer[len] = '\0';
    }
#endif

	return std::string(buffer);
}

std::string getProjectPath() {
	fs::path exePath = fs::path(getProgramPath());
	fs::path projectRoot = exePath.parent_path().parent_path();
	return projectRoot.string();
}