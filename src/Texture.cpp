#include "Texture.h"
#include "stb_image.h"

Texture::Texture() {
    ID = 0;
    width = 0;
    height = 0;
    nrChannels = 0;
    initialized = false;
}

Texture::Texture(const GLchar* imgPath, GLint internalFormat, GLenum format, GLint sWrapMode, GLint tWrapMode, GLint minFilterMode, GLint magFilterMode)
{
    this->ID = 0;
    glGenTextures(1, &(this->ID));
    glBindTexture(GL_TEXTURE_2D, this->ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapMode);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterMode);

    unsigned char* image = stbi_load(imgPath, &(this->width), &(this->height), &(this->nrChannels), 0);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);
    initialized = true;
}

Texture::Texture(const GLchar* imgPath, GLint internalFormat, GLenum format, GLint wrapMode, GLint filterMode) : 
    Texture(imgPath, internalFormat, format, wrapMode, wrapMode, filterMode, filterMode) {}

void Texture::use() const
{
    if (initialized)
        glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::use(GLuint textureUnitNum) const
{
    if (initialized) {
        glActiveTexture(GL_TEXTURE0 + textureUnitNum);
        use();
    }
}