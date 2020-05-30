#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
    ID = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(ID);
}

void ShaderProgram::use() {
    glUseProgram(ID);
}

void ShaderProgram::attach(Shader shader) {
    glAttachShader(ID, shader.getID());
}
void ShaderProgram::attach(unsigned int shaderID) {
    glAttachShader(ID, shaderID);
}

void ShaderProgram::link() {
    glLinkProgram(ID);
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void ShaderProgram::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderProgram::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string& name, float value1, float value2) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), value1, value2);
}
void ShaderProgram::setVec3(const std::string& name, float value1, float value2, float value3) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}
void ShaderProgram::setVec4(const std::string& name, float value1, float value2, float value3, float value4) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}

void ShaderProgram::setVec2(const std::string& name, glm::vec2 values) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), values.x, values.y);
}
void ShaderProgram::setVec3(const std::string& name, glm::vec3 values) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), values.x, values.y, values.z);
}
void ShaderProgram::setVec4(const std::string& name, glm::vec4 values) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), values.x, values.y, values.z, values.w);
}

void ShaderProgram::setUint(const std::string& name, unsigned int value) const
{
    glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}
void ShaderProgram::setUvec2(const std::string& name, unsigned int value1, unsigned int value2) const
{
    glUniform2ui(glGetUniformLocation(ID, name.c_str()), value1, value2);
}
void ShaderProgram::setUvec3(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3) const
{
    glUniform3ui(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}
void ShaderProgram::setUvec4(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4) const
{
    glUniform4ui(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}