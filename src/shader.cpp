#include <shader.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

using namespace std;

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertexFile.open(vertexShaderPath);
        fragmentFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vertexFile.rdbuf();
        fShaderStream << fragmentFile.rdbuf();
        vertexFile.close();
        fragmentFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    m_Id = glCreateProgram();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    int success;
    char infoLog[1024];

    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
        std::cerr << "[ERROR]::VERTEX_SHADER_COMPILATION_FAILED: " << infoLog << std::endl;
    }

    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
        std::cerr << "[ERROR]::FRAGMENT_SHADER_COMPILATION_FAILED:: " << infoLog << std::endl;
    }

    glAttachShader(m_Id, vertexShader);
    glAttachShader(m_Id, fragmentShader);
    glLinkProgram(m_Id);

    glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_Id, 1024, nullptr, infoLog);
        std::cerr << "[ERROR]::PROGRAM_LINKING_FAILED:: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->use();
}

void Shader::use() const {
    glUseProgram(m_Id);
}

//---------------------------------------Bool-----------------------------------------------------------------------
void Shader::setBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(m_Id, name), (int)value);
}

//---------------------------------------Int-----------------------------------------------------------------------
void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(m_Id, name), value);
}

//---------------------------------------Float-----------------------------------------------------------------------
void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(m_Id, name), value);
}

//---------------------------------------Vec2-----------------------------------------------------------------------
void Shader::setVec2(const char* name, float x, float y) const {
    glUniform2f(glGetUniformLocation(m_Id, name), x, y);
}

void Shader::setVec2(const char* name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(m_Id, name), 1, &value[0]);
}

//---------------------------------------Vec3-----------------------------------------------------------------------
void Shader::setVec3(const char* name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(m_Id, name), x, y, z);
}

void Shader::setVec3(const char* name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_Id, name), 1, &value[0]);
}

//---------------------------------------Vec4-----------------------------------------------------------------------
void Shader::setVec4(const char* name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(m_Id, name), x, y, z, w);
}

void Shader::setVec4(const char* name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_Id, name), 1, &value[0]);
}

//---------------------------------------Mat2-----------------------------------------------------------------------
void Shader::setMat2(const char* name, const glm::mat2& value) const {
    glUniformMatrix2fv(glGetUniformLocation(m_Id, name), 1, GL_FALSE, &value[0][0]);
}

//---------------------------------------Mat3-----------------------------------------------------------------------
void Shader::setMat3(const char* name, const glm::mat3& value) const {
    glUniformMatrix3fv(glGetUniformLocation(m_Id, name), 1, GL_FALSE, &value[0][0]);
}

//---------------------------------------Mat4-----------------------------------------------------------------------
void Shader::setMat4(const char* name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_Id, name), 1, GL_FALSE, &value[0][0]);
}
