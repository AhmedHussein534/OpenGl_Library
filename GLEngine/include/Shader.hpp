#pragma once

#include <string>
#include "Renderer.hpp"
#include <iostream>
#include <unordered_map>

namespace GL_ENGINE
{
	namespace SetUniformDetailed
	{
		template <class ...Types>
		void setUniformValue(int location, Types... values);
	};

	class Shader
	{
	public:
		Shader(const std::string &vertexShader, const std::string &fragmentShader);

		void bind();

		void unbind();

		template <class ...Types>
		void setUniformValue(const std::string& name, Types... values)
		{
			SetUniformDetailed::setUniformValue(getUniformLocation(name), values...);
		}

		int getUniformLocation(const std::string &uniformName);

		~Shader();

	private:
		unsigned int compileShader(unsigned int type, const std::string& source);
		unsigned int program;
		std::string m_vertexShader;
		std::string m_fragmentShader;
		std::unordered_map<std::string, int> locationCache;

	};
}
