#include "shader.h"


Shader::Shader(const std::string& shaderPath, unsigned int forceShaderType) : ID_ptr(std::make_shared<ShaderID_>(shaderPath, forceShaderType)) {}

unsigned int Shader::getID()
{
    return ID_ptr->ID;
}

ShaderID_::ShaderID_(const std::string& shaderPath, unsigned int forceShaderType)
{
    std::string shaderCode;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        shaderFile.open(shaderPath);
        std::stringstream shaderStream;
        // read file's buffer contents into stream
        shaderStream << shaderFile.rdbuf();
        // close file handler
        shaderFile.close();
        // convert stream into string
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* rawShaderCode = shaderCode.c_str();

    int success;
    char infoLog[512];

    ID = glCreateShader(forceShaderType);
    glShaderSource(ID, 1, &rawShaderCode, NULL);
    glCompileShader(ID);
    // print compile errors if any
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

ShaderID_::~ShaderID_()
{
    glDeleteShader(ID);
}

