#pragma once
#include <string>
#include "shader.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class ShaderProgram
{
public:
    unsigned int ID; // Handle for the OpenGL program object

    ShaderProgram(); // constructor creates a new OpenGL program object
    
    ~ShaderProgram(); // Deletes program
    
    // TODO: Copy & move for shaderprogram object
    ShaderProgram(ShaderProgram const&) = delete;
    void operator=(ShaderProgram const& x) = delete;

    void use(); // use/activate the shader program

    void attach(Shader shader);
    void attach(unsigned int shaderID);

    void link();

    // functions to set uniforms of the active shader program. 
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

    void setVec2(const std::string& name, float value1, float value2) const;
    void setVec3(const std::string& name, float value1, float value2, float value3) const;
    void setVec4(const std::string& name, float value1, float value2, float value3, float value4) const;

    void setVec2(const std::string& name, glm::vec2 values) const;
    void setVec3(const std::string& name, glm::vec3 values) const;
    void setVec4(const std::string& name, glm::vec4 values) const;

    void setUint(const std::string& name, unsigned int value) const;

    void setUvec2(const std::string& name, unsigned int value1, unsigned int value2) const;
    void setUvec3(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3) const;
    void setUvec4(const std::string& name, unsigned int value1, unsigned int value2, unsigned int value3, unsigned int value4) const;
};

