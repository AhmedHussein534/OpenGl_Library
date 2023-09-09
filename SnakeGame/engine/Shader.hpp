#pragma once

#include <string>
#include <GL/glew.h>
#include "Renderer.hpp"
#include <iostream>

class Shader
{
public:
	Shader(std::string vertexShader, std::string fragmentShader);

	void bind();

	void unbind();

	template <class ...Types>
	void setUniformValue(const std::string& name, Types... values)
	{
		std::cout << "Unknown function: " << __name__ << std::endl;
	}

	template<>
	void setUniformValue(const std::string& name, float f1)
	{
		GLCall(int location = glGetUniformLocation(program, name.c_str()));
		GLCall(glUniform1f(location, f1));
	}

	template<>
	void setUniformValue(const std::string& name, float f1, float f2)
	{
		GLCall(int location = glGetUniformLocation(program, name.c_str()));
		if (location == -1)
		{
			std::cout << "Uniform: " << name << " not found!" << std::endl;
			ASSERT(0);
		}
		GLCall(glUniform2f(location, f1, f2));
	}

	template<>
	void setUniformValue(const std::string& name, float f1, float f2, float f3)
	{
		GLCall(int location = glGetUniformLocation(program, name.c_str()));
		if (location == -1)
		{
			std::cout << "Uniform: " << name << " not found!" << std::endl;
			ASSERT(0);
		}
		GLCall(glUniform3f(location, f1, f2, f3));
	}

	template<>
	void setUniformValue(const std::string& name, float f1, float f2, float f3, float f4)
	{
		GLCall(int location = glGetUniformLocation(program, name.c_str()));
		if (location == -1)
		{
			std::cout << "Uniform: " << name << " not found!" << std::endl;
			ASSERT(0);
		}
		GLCall(glUniform4f(location, f1, f2, f3, f4));
	}

	template<>
	void setUniformValue(const std::string& name, int32_t f1)
	{
		GLCall(int location = glGetUniformLocation(program, name.c_str()));
		if (location == -1)
		{
			std::cout << "Uniform: " << name << " not found!" << std::endl;
			ASSERT(0);
		}
		GLCall(glUniform1i(location, f1));
	}

	~Shader();

private:
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int program;
	std::string m_vertexShader;
	std::string m_fragmentShader;
};