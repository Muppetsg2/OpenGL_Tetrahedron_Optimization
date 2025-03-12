#pragma once

class Texture {
private:
    bool initialized = false;
public:
    // ID texture object 
    GLuint ID;
    int width = 0;
    int height = 0;
    int nrChannels = 0;

    Texture();

    // konstruktor czyta plik tekstury  
    Texture(const GLchar* imgPath, GLint internalFormat, GLenum format, GLint sWrapMode, GLint tWrapMode, GLint minFilterMode, GLint magFilterMode);
    Texture(const GLchar* imgPath, GLint internalFormat, GLenum format, GLint wrapMode, GLint filterMode);
    // aktywuj teksture
    void use() const;
    void use(GLuint textureUnitNum) const;
};