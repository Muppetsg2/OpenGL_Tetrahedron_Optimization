#include <Shader.hpp>

Shader::Shader() {
    ID = 0;
    initialized = false;
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    // 1. pobierz kod �r�d�owy Vertex/Fragment Shadera z filePath  
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // zapewnij by obiekt ifstream m�g� rzuca� wyj�tkami  
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // otw�rz pliki  
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // zapisz zawarto�� bufora pliku do strumieni  
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // zamknij uchtywy do plik�w  
        vShaderFile.close();
        fShaderFile.close();
        // zamie� strumie� w �a�cuch znak�w  
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. skompiluj shadery  
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader  
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // wypisz b��dy kompilacji, je�li s� jakie�  
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Fragment Shader  
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // wypisz b��dy kompilacji, je�li s� jakie�  
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Program Object  
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // wypisz b��dy linkowania, je�li s� jakie�
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // usu� obiekty shader'�w, kt�re s� ju� powi�zane  
    // z Program Object - nie b�d� nam ju� potrzebne  
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    initialized = true;
}

Shader::Shader(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath)
{
    // 1. pobierz kod �r�d�owy Vertex/Fragment Shadera z filePath  
    std::string vertexCode;
    std::string geometryCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream gShaderFile;
    std::ifstream fShaderFile;
    // zapewnij by obiekt ifstream m�g� rzuca� wyj�tkami  
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // otw�rz pliki  
        vShaderFile.open(vertexPath);
        gShaderFile.open(geometryPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, gShaderStream, fShaderStream;
        // zapisz zawarto�� bufora pliku do strumieni  
        vShaderStream << vShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // zamknij uchtywy do plik�w  
        vShaderFile.close();
        gShaderFile.close();
        fShaderFile.close();
        // zamie� strumie� w �a�cuch znak�w  
        vertexCode = vShaderStream.str();
        geometryCode = gShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. skompiluj shadery  
    unsigned int vertex, geometry, fragment;
    int success;
    char infoLog[512];

    // Vertex Shader  
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // wypisz b��dy kompilacji, je�li s� jakie�  
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Geometry Shader  
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    // wypisz b��dy kompilacji, je�li s� jakie�  
    glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometry, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Fragment Shader  
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // wypisz b��dy kompilacji, je�li s� jakie�  
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // Program Object  
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, geometry);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    // wypisz b��dy linkowania, je�li s� jakie�
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // usu� obiekty shader'�w, kt�re s� ju� powi�zane  
    // z Program Object - nie b�d� nam ju� potrzebne  
    glDeleteShader(vertex);
    glDeleteShader(geometry);
    glDeleteShader(fragment);
    initialized = true;
}

void Shader::use() const
{
    if (initialized)
        glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    if (initialized)
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    if (initialized)
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    if (initialized)
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    if (initialized)
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    if (initialized)
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMatrix4x4(const std::string& name, const glm::mat4& value) const
{
    if (initialized)
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}