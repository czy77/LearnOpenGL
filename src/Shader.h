//
// Created by Gooma on 2021/5/31.
//
#pragma once

#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



class Shader {

public:
    //程序ID
    unsigned int ID;

    //构造器
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const char* geometryPath);


    //使用/激活程序
    void use();


    //uniform 工具函数
    void setBool(const std::string &name, bool value) const;

    void setInt(const std::string &name, int value) const;

    void setFloat(const std::string &name, float value) const;
//    void setVec2(const std::string &name, const glm::vec2 &value) const;


private:
    void checkCompileErrors(GLuint shader, std::string type);
};


Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const char* geometryPath = nullptr) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    //确保文件对象可以正确抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream,fShaderStream;

        //把文件内容读入流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //关闭文件处理
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if(geometryPath != nullptr){
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::fstream::failure& e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READY" << std::endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    unsigned int vertex,fragment;

    //顶点着色器
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");


    //片段着色器
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    //几何着色器(如果有点话)
    unsigned int geometry;
    if(geometryPath != nullptr){
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, nullptr);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }

    //shader程序
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geometryPath != nullptr) {
        glAttachShader(ID, geometry);
    }
    glLinkProgram(ID);

    checkCompileErrors(ID, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr) {
        glDeleteShader(geometry);
    }
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);

}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infolog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infolog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type : " << type << "\n" << infolog
                      << "\n -- ------------------------------- --" << std::endl;

        }else{
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infolog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infolog
                          << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }


    }

}


