#pragma once

#include <string>
#include <unordered_map>

class TextureLibrary
{
public:
    unsigned int load(const std::string& cardName);
    unsigned int get(const std::string& cardName) const;

private:
    std::unordered_map<std::string, unsigned int> textures;
};