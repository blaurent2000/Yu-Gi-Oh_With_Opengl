#include "TextureLibrary.hpp"

#include <glad/gl.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

unsigned int TextureLibrary::load(const std::string& cardName)
{
    std::string path = "textures/cards/" + cardName + ".jpg";

    int width;
    int height;
    int nrChannels;

    unsigned char* data = stbi_load(
        path.c_str(),
        &width,
        &height,
        &nrChannels,
        4
    );

    if (!data) {
        std::cerr << "Erreur : impossible de charger " << path << '\n';
        return 0;
    }

    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    textures[cardName] = texture;

    return texture;
}

unsigned int TextureLibrary::get(const std::string& cardName) const
{
    auto it = textures.find(cardName);

    if (it == textures.end())
        return 0;

    return it->second;
}