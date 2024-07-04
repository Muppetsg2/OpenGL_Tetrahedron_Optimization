#pragma once

#include <glad/glad.h>// do³¹cz glad, by móc korzystaæ w wszystkich wymaganych przez OpenGL funkcji</glad>
// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
private:
    bool initialized = false;
public:
    // ID program object  
    GLuint ID;

    Shader();

    // konstruktor czyta plik shadera z dysku i tworzy go  
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // aktywuj shader  
    void use() const;
    // funkcje operuj¹ce na uniformach  
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMatrix4x4(const std::string& name, const glm::mat4& value) const;
};