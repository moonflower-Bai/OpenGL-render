//
// Created by Jingren Bai on 25-8-11.
//

#ifndef LEARNOPENGL_GETPROGRAMPATH_H
#define LEARNOPENGL_GETPROGRAMPATH_H

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <unistd.h>
#endif
#include "log.cpp"
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

std::string getProgramPath();
std::string getProjectPath();

#endif // LEARNOPENGL_GETPROGRAMPATH_H
