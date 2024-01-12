#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>  
#include <iostream>

class Shader{
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath) {
            // 1. retrieve the vertex/fragment source code from filePath
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            // ensure ifstream objects can throw exceptions:
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                // read file's buffer contents into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                // close file handlers
                vShaderFile.close();
                fShaderFile.close();
                // convert stream into string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            } catch (std::ifstream::failure& e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
            }
            const char* vertexShaderSource = vertexCode.c_str();
            const char * fragmentShaderSource = fragmentCode.c_str();

            // 2. Complie Shaders
            unsigned int vertexShader, fragmentShader;
            // create vertex shader object
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            // attach shader source code to shader object
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            // compile shader
            glCompileShader(vertexShader);

            // create fragment shader object
            fragmentShader  = glCreateShader(GL_FRAGMENT_SHADER);
            // attach shader source code to shader object
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            // compile shader
            glCompileShader(fragmentShader);

            ID = glCreateProgram();

            // attach and link reviously compiled shaders to the program object
            glAttachShader(ID, vertexShader);
            glAttachShader(ID, fragmentShader);
            glLinkProgram(ID);

            // delete shader objects after using
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        void use() {
            glUseProgram(ID); 
        }

        void setBool(std::string &name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);     
        }
        void setFloat(std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
        }
        
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};


#endif