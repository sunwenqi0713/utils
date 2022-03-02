//
// Created by vikey on 2022/3/2.
//

#include "file.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <climits>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <cstring>

const int MAX_FILE_LENGTH = 32 * 1024 * 1024;

namespace utils {
namespace base {

bool PathExists(const std::string &path)
{
  struct stat info;
  return stat(path.c_str(), &info) == 0;
}

bool LoadStringFromFile(const std::string &filePath, std::string &content)
{
  std::ifstream file(filePath.c_str());
  if (!file.is_open()) {
    std::cout << "open file failed! filePath: " << filePath.c_str() << std::endl;
    return false;
  }

  file.seekg(0, std::ios::end);
  const long fileLength = file.tellg();
  if (fileLength > MAX_FILE_LENGTH) {
    std::cout << "invalid file length: " << fileLength << std::endl;
    return false;
  }

  content.clear();
  file.seekg(0, std::ios::beg);
  std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), back_inserter(content));
  return true;
}

std::string GetFileNameByFd(const int fd)
{
  if (fd <= 0) {
    return std::string();
  }

  std::string fdPath = "/proc/self/fd/" + std::to_string(fd);
  char fileName[PATH_MAX + 1] = {0};

  int ret = readlink(fdPath.c_str(), fileName, PATH_MAX);
  if (ret < 0 || ret > PATH_MAX) {
    std::cout << "Get fileName failed, ret is: " << ret << std::endl;
    return std::string();
  }
  fileName[ret] = '\0';
  return std::string(fileName);
}

bool LoadStringFromFdToFile(int fd, std::string &content)
{
  std::string fileName = GetFileNameByFd(fd);
  if (fileName.empty()) {
    std::cout << "LoadStringFromFd get file name by fd failed!" << std::endl;
    return false;
  }

  if (!LoadStringFromFile(fileName, content)) {
    std::cout << "LoadStringFromFd get string from file failed!" << std::endl;
    return false;
  }
  return true;
}

bool LoadStringFromFd(int fd, std::string &content)
{
  if (fd <= 0) {
    std::cout << "invalid fd: " << fd << std::endl;
    return false;
  }

  const long fileLength = lseek(fd, 0, SEEK_END);
  if (fileLength > MAX_FILE_LENGTH) {
    std::cout << "invalid file length: " << fileLength << std::endl;
    return false;
  }

  // lseek is not support the linux file node
  if (fileLength < 0) {
    return LoadStringFromFdToFile(fd, content);
  }

  if (fileLength == 0) {
    return true;
  }

  content.resize(fileLength);
  int loc = lseek(fd, 0, SEEK_SET);
  if (loc == -1) {
    std::cout << "lseek file to begin failed! " << std::endl;
    return false;
  }

  const long len = read(fd, (void *) content.data(), fileLength);
  if (len != fileLength) {
    std::cout << "the length read from file is not equal to fileLength!len: " << len << ",fileLen: " << fileLength
              << std::endl;
    return false;
  }

  return true;
}

bool SaveStringToFile(const std::string &filePath, const std::string &content, bool truncated /*= true*/)
{
  if (content.empty()) {
    std::cout << "content is empty, no need to save!" << std::endl;
    return true;
  }

  std::ofstream file;
  if (truncated) {
    file.open(filePath.c_str(), std::ios::out | std::ios::trunc);
  } else {
    file.open(filePath.c_str(), std::ios::out | std::ios::app);
  }

  if (!file.is_open()) {
    std::cout << "open file failed! filePath: " << filePath.c_str() << std::endl;
    return false;
  }

  file.write(content.c_str(), content.length());
  if (file.fail()) {
    std::cout << "write content to file failed! file: " << filePath.c_str() << ", content: " << content.c_str()
              << std::endl;
    return false;
  }
  return true;
}

bool SaveStringToFd(int fd, const std::string &content)
{
  if (fd <= 0) {
    std::cout << "invalid fd: " << fd << std::endl;
    return false;
  }

  if (content.empty()) {
    std::cout << "content is empty, no need to save!" << std::endl;
    return true;
  }

  const long len = write(fd, content.c_str(), content.length());
  if (len < 0) {
    std::cout << "write file failed! errno: " << errno << ", err:" << strerror(errno) << std::endl;
    return false;
  }

  if ((unsigned long) len != content.length()) {
    std::cout << "the length write to file is not equal to fileLength! len: " << len << ", fileLen:" << content.length()
              << std::endl;
    return false;
  }

  return true;
}

}
}