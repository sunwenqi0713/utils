//
// Created by vikey on 2022/3/2.
//

#include "directory.h"
#include <iostream>
#include <climits>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace utils {
namespace base {

std::string GetCurrentProcFullFileName()
{
  char procFile[PATH_MAX + 1] = {0};
  int ret = readlink("/proc/self/exe", procFile, PATH_MAX);
  if (ret < 0 || ret > PATH_MAX) {
    std::cout << "Get proc name failed, ret is: " << ret << std::endl;
    return std::string();
  }
  procFile[ret] = '\0';
  return std::string(procFile);
}

std::string GetCurrentProcPath()
{
  return ExtractFilePath(GetCurrentProcFullFileName());
}

std::string ExtractFilePath(const std::string &fileFullName)
{
  return std::string(fileFullName).substr(0, fileFullName.rfind("/") + 1);
}

std::string ExtractFileName(const std::string &fileFullName)
{
  return std::string(fileFullName).substr(fileFullName.rfind("/") + 1, fileFullName.size());
}

std::string ExtractFileExt(const std::string &fileName)
{
  std::string::size_type pos = fileName.rfind(".");
  if (pos == std::string::npos) {
    return "";
  }

  return std::string(fileName).substr(pos + 1, fileName.size());
}

std::string ExcludeTrailingPathDelimiter(const std::string &path)
{
  if (path.rfind("/") != path.size() - 1) {
    return path;
  }

  if (!path.empty()) {
    return path.substr(0, (int) path.size() - 1);
  }

  return path;
}

std::string IncludeTrailingPathDelimiter(const std::string &path)
{
  if (path.rfind("/") != path.size() - 1) {
    return path + "/";
  }

  return path;
}

void GetDirFiles(const std::string &path, std::vector<std::string> &files)
{
  std::string pathStringWithDelimiter;
  DIR *dir = opendir(path.c_str());
  if (dir == nullptr) {
    return;
  }

  while (true) {
    struct dirent *ptr = readdir(dir);
    if (ptr == nullptr) {
      break;
    }

    // current dir OR parent dir
    if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0)) {
      continue;
    } else if (ptr->d_type == DT_DIR) {
      pathStringWithDelimiter = IncludeTrailingPathDelimiter(path) + std::string(ptr->d_name);
      GetDirFiles(pathStringWithDelimiter, files);
    } else {
      files.push_back(IncludeTrailingPathDelimiter(path) + std::string(ptr->d_name));
    }
  }
  closedir(dir);
}

}
}