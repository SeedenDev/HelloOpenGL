#pragma once

#include <string>

std::string ReadFileAsString(const std::string& filepath)
{
    // C++ way of reading file, on the basis of how to do it with the C API (could be a little bit quicker)
    std::ifstream stream(filepath);
    std::string contents;
    stream.seekg(0, std::ios::end);
    contents.resize(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(&contents[0], contents.size());
    stream.close();
    return contents;
}