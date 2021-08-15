#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    //着色程序ID
    unsigned int ID;

    //构造器读取并构建着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    
    //使用着色程序
    void use(){ glUseProgram(ID); };

    //uniform工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;
    void setVec3(const std::string &name, float value1, float value2, float value3) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif