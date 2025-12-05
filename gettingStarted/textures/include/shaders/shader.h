#pragma once 

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader 
{
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath)
        {
            // Retrieve vertex and fragment shader from file path
            std::string vertexCode;
            std::string fragCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            // Set up exceptions for ifstream object
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                // Open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;

                // Read the files
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                // Close files
                vShaderFile.close();
                fShaderFile.close();

                // Convert to string
                vertexCode = vShaderStream.str();
                fragCode = fShaderStream.str();
            }
            catch(const std::ifstream::failure e)
            {
                std::cerr << "ERROR: Shader File Not Succesfully Read \n" << e.what();
            }

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragCode.c_str();

            // Compile shaders 
            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            // Vertex Shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, nullptr);
            glCompileShader(vertex);

            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
                std::cout << "Error Vertex Shater Compilation: " << infoLog << "\n";
            }

            // Fragment SHader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, nullptr);
            glCompileShader(fragment);
            
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
                std::cout << "Error Fragment Shader Compilation: " << infoLog << "\n";
            }

            // Attach shaders
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(ID, 512, nullptr, infoLog);
                std::cout << "Error Linking shaders: " << infoLog << "\n";
            }

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use()
        {
            glUseProgram(ID);
        }

        void setBool(const std::string& name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }

        void setInt(const std::string& name, int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        void setFloat(const std::string& name, float value) const
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
};