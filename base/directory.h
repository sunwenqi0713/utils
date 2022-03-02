//
// Created by vikey on 2022/3/2.
//

#ifndef UTILS_DIRECTORY_H
#define UTILS_DIRECTORY_H

#include <string>
#include <vector>

namespace utils {
namespace base {

/**
* The GetCurrentProcFullFileName function get the current process exe name.
*/
std::string GetCurrentProcFullFileName();

/**
* The GetCurrentProcPath function get the current process exe path.
*/
std::string GetCurrentProcPath();

/**
* The ExtractFilePath function extract the input file path.
*/
std::string ExtractFilePath(const std::string &fileFullName);

/**
* The ExtractFilePath function extract the input file name.
*/
std::string ExtractFileName(const std::string &fileFullName);

/**
* The ExtractFileExt function extract the input file name type.
*/
std::string ExtractFileExt(const std::string &fileName);

/**
 * The ExcludeTrailingPathDelimiter function exclude the end '/' from the strPath,
 * return the path without the end '/'.
 */
std::string ExcludeTrailingPathDelimiter(const std::string &path);

/**
 * The IncludeTrailingPathDelimiter function include the end '/' from the strPath,
 * return the path with the end '/'.
 */
std::string IncludeTrailingPathDelimiter(const std::string &path);

/**
 * The GetDirFiles function get all files in the path.
 */
void GetDirFiles(const std::string &path, std::vector<std::string> &files);

}
}

#endif//UTILS_DIRECTORY_H
