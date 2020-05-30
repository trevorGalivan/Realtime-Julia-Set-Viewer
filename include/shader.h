#pragma once

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

// Should not be used by client code directly. Helper class that allows multiple instances of 'Shader' to to reference the same shader
class ShaderID_
{
public:
    unsigned int ID; // Handle for the OpenGL shader object
    
    ShaderID_(const std::string& shaderPath, unsigned int shaderType); // Loads the source code of a new shader, and compiles it
    ~ShaderID_();
};

class Shader
{
    friend class ShaderProgram;
public:
    // constructor makes a shared pointer to a new ShaderID_ 
    Shader(const std::string& shaderPath, unsigned int shaderType);
private:
    unsigned int getID(); // Used by friend ShaderProgram
    
    std::shared_ptr<ShaderID_> ID_ptr; // Shared pointer to ID allows for safe shallow copies
    
};

