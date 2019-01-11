#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>

#include "ad_block_client.h"

std::string stripExtenstion(const std::string &fileName)
{
    return fileName.substr(0, fileName.find_last_of("."));
}

std::string getFileContents(const char *filename)
{
    std::ifstream in(filename, std::ios::in);
    if (in)
    {
        std::ostringstream contents;
        contents << in.rdbuf();
        in.close();
        return (contents.str());
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

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: fserialize [input directory of filters]" << std::endl;
        return 0;
    }

    std::string input = argv[1];

    if (input.find_last_of("/") != input.length() - 1)
    {
        input.append("/");
    }

    AdBlockClient client;

    for (auto &item : getFilesInPath(input))
    {
        std::string tmp = input;

        std::cout << "Parsing: " << tmp.append(item) << std::endl;

        std::string list = getFileContents(tmp.c_str());

        client.parse(list.c_str());
    }

    int size;
    char *buffer = client.serialize(&size);
    writeFile("./filter.dat", buffer, size);

    return 0;
}
