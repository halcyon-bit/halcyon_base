#include "base/file/file_opt.h"

#if defined USE_CPP11 || defined USE_CPP14

#ifdef LINUX
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#elif defined WINDOWS
#include <io.h>
#include <direct.h>
#endif

#else
#include <filesystem>  // c++17
#endif

BASE_BEGIN_NAMESPACE

bool file::exists(string_view filename)
{
#if defined USE_CPP11 || defined USE_CPP14

#ifdef LINUX
    return access(filename.data(), 0) == 0;
#elif defined WINDOWS
    return _access(filename.data(), 0) == 0;
#endif

#else
    return std::filesystem::exists(filename.data());
#endif
}

bool file::createDir(string_view dir)
{
    if (dir[dir.size() - 1] == '/' || dir[dir.size() - 1] == '\\') {
        return false;
    }
#if defined USE_CPP11 || defined USE_CPP14
    bool ret = false;
    size_t start = dir.find("/");
    if (start == -1) {
        start = dir.find("\\");
    }
    if (start == -1) {
        return false;
    }

    size_t size = dir.size();
    for (size_t i = start + 1; i < size; ++i) {
        if (dir[i] == '\\' || dir[i] == '/') {
            std::string tmp(dir.substr(0, i).data(), i);
#ifdef LINUX
            ret = mkdir(tmp.data(), 0777) == 0;
#elif defined WINDOWS
            ret = _mkdir(tmp.data()) == 0;
#endif
        }
        else if (i == size - 1) {
#ifdef LINUX
            ret = mkdir(dir.data(), 0777) == 0;
#elif defined WINDOWS
            ret = _mkdir(dir.data()) == 0;
#endif
        }
    }
    return ret;
#else
    return std::filesystem::create_directories(dir.data());
#endif
}

#if defined USE_CPP11 || defined USE_CPP14
#ifdef LINUX
static bool removeAll(string_view dir)
{
    DIR* new_dir = opendir(dir.data());
    if (nullptr == new_dir) {
        return false;
    }

    std::string path(dir.data());
    path.append("/");
    struct dirent* dirp;
    // 删除该文件夹下的所有文件(夹)
    while ((dirp = readdir(new_dir)) != nullptr) {
        if (dirp->d_type == DT_DIR) {
            if (strcmp(dirp->d_name, ".") == 0
                || strcmp(dirp->d_name, "..") == 0)
                continue;

            path.erase(path.rfind("/") + 1);
            path.append(dirp->d_name);
            // 文件夹，再次遍历
            if (!removeAll(path)) {
                return false;
            }
        }
        else if (dirp->d_type == DT_REG) {
            // 文件
            path.erase(path.rfind("/") + 1);
            path.append(dirp->d_name);
            if (remove(path.c_str()) != 0) {
                return false;
            }
        }
    }
    closedir(new_dir);
    // 删除空文件夹
    return rmdir(dir.data()) == 0;
}
#elif defined WINDOWS
static bool removeAll(string_view dir)
{
    std::string new_dir(dir.data());
    new_dir.append("\\*.*");

    struct _finddata_t file_info;
    intptr_t handle = _findfirst(new_dir.c_str(), &file_info);
    if (handle == -1) {
        return false;
    }

    // 删除该文件夹下的所有文件(夹)
    do {
        if (file_info.attrib & _A_SUBDIR) {
            if (strcmp(file_info.name, ".") == 0
                || strcmp(file_info.name, "..") == 0)
                continue;

            new_dir.erase(new_dir.rfind('\\') + 1);
            new_dir.append(file_info.name);
            // 文件夹，再次遍历
            if (!removeAll(new_dir)) {
                return false;
            }
        }
        else {
            // 文件
            new_dir.erase(new_dir.rfind('\\') + 1);
            new_dir.append(file_info.name);
            if (remove(new_dir.c_str()) != 0) {
                _findclose(handle);
                return false;
            }
        }
    } while (!_findnext(handle, &file_info));

    _findclose(handle);
    // 删除空文件夹
    return _rmdir(dir.data()) == 0;
}
#endif
#endif

bool file::removeDir(string_view dir)
{
#if defined USE_CPP11 || defined USE_CPP14
    return removeAll(dir);
#else
    return std::filesystem::remove_all(dir.data());
#endif
}

#if defined USE_CPP11 || defined USE_CPP14
#ifdef LINUX
static void listAll(string_view dir, std::vector<std::string>& dirs, std::vector<std::string>& files)
{
    DIR* new_dir = opendir(dir.data());
    if (nullptr == new_dir) {
        return;
    }

    std::string path(dir.data());
    path.append("/");
    struct dirent* dirp;

    while ((dirp = readdir(new_dir)) != nullptr) {
        if (dirp->d_type == DT_DIR) {
            if (strcmp(dirp->d_name, ".") == 0
                || strcmp(dirp->d_name, "..") == 0)
                continue;

            dirs.push_back(dirp->d_name);
        }
        else if (dirp->d_type == DT_REG) {
            // 文件
            files.push_back(dirp->d_name);
        }
    }
    closedir(new_dir);
}
#elif defined WINDOWS
static void listAll(string_view dir, std::vector<std::string>& dirs, std::vector<std::string>& files)
{
    std::string new_dir(dir.data());
    new_dir.append("\\*.*");

    struct _finddata_t file_info;
    intptr_t handle = _findfirst(new_dir.c_str(), &file_info);
    if (handle == -1) {
        return;
    }

    // 遍历该文件夹下的所有文件(夹)
    do {
        if (file_info.attrib & _A_SUBDIR) {
            if (strcmp(file_info.name, ".") == 0
                || strcmp(file_info.name, "..") == 0)
                continue;

            dirs.push_back(file_info.name);
        }
        else {
            // 文件
            files.push_back(file_info.name);
        }
    } while (!_findnext(handle, &file_info));

    _findclose(handle);
}
#endif
#endif

void file::listDir(string_view dir, std::vector<std::string>& dirs, std::vector<std::string>& files)
{
#if defined USE_CPP11 || defined USE_CPP14
    listAll(dir, dirs, files);
#else
    if (!std::filesystem::exists(dir.data()) || !std::filesystem::is_directory(dir.data())) {
        return;
    }

    for (auto& each : std::filesystem::directory_iterator(dir.data())) {
        auto filename = each.path().filename();
        if (std::filesystem::is_directory(each)) {
            if (filename == "." || filename == "..")
                continue;

            dirs.push_back(filename.string());
        }
        else {
            files.push_back(filename.string());
        }
    }
#endif
}

bool file::removeFile(string_view file)
{
#if defined USE_CPP11 || defined USE_CPP14
    return remove(file.data()) == 0;
#else
    return std::filesystem::remove(file.data());
#endif
}

BASE_END_NAMESPACE