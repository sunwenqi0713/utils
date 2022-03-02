//
// Created by vikey on 2022/3/2.
//

#ifndef UTILS_BASE_FILE_H_
#define UTILS_BASE_FILE_H_

#include <string>

namespace utils {
namespace base {

bool LoadStringFromFile(const std::string& filePath, std::string& content);
bool SaveStringToFile(const std::string& filePath, const std::string& content, bool truncated = true);
bool LoadStringFromFd(int fd, std::string& content);
bool SaveStringToFd(int fd, const std::string& content);

}
}
#endif //UTILS_BASE_FILE_H_
