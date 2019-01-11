#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <atlstr.h>

#include "ad_block_client.h"

std::string stripExtenstion(const std::string &fileName)
{
    return fileName.substr(0, fileName.find_last_of("."));
}

std::string getFileContents(const char *filename)
{
  std::ifstream in(filename, std::ios::in);
  if (in) {
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    return(contents.str());
  }
  throw(errno);
}

void writeFile(const char *filename, const char *buffer, int length)
{
    std::ofstream out(filename, std::ios::out | std::ios::binary);

    if (out)
    {
        out.write(buffer, length);
        out.close();
        return;
    }

    throw(errno);
}

std::vector<std::string> getFilesInPath(std::string path)
{
    std::vector<std::string> fileNames;
    LPCSTR searchPath = path.append("/*.*").c_str();

    WIN32_FIND_DATAA fd;
    HANDLE hFind = ::FindFirstFileA(searchPath, &fd);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                fileNames.push_back(fd.cFileName);
            }
        } while (::FindNextFileA(hFind, &fd));
    }

    return fileNames;
}

void serializeFilter(std::string path, std::string output)
{
    AdBlockClient client;
    client.parse(getFileContents(path.c_str()).c_str());

    std::string fileName = path.substr(path.find_last_of("/") + 1, path.length());

    int size;
    char *buffer = client.serialize(&size);

    if (output.find_last_of("/") != output.length()) {
        output = output.append("/");
    }

    if (output.find_first_of("./") != 0) {
        std::string prefix = "./";
        output = prefix.append(output);
    }

    std::string outputPath = output.append(stripExtenstion(fileName)).append(".dat");

    writeFile(outputPath.c_str(), buffer, size);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: fcompile [input directory of filters] [output directory]" << std::endl;
        return 0;
    }

    std::string input = argv[1];
    std::string output = argv[2];

    for (auto &item : getFilesInPath(input))
    {
        if (input.find_last_of("/") != input.length() - 1)
        {
            input.append("/");
        }

        serializeFilter(input.append(item), output);
    }

    return 0;
}
