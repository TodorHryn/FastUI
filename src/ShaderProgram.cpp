#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\string_cast.hpp>
#include "Util.h"

#define CHECK_SHADER(shader)																		\
	do {																							\
		GLint success;																				\
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);											\
		if (!success) {																				\
			std::cout << "Shader compilation failed (line " << __LINE__ << "):" << std::endl;		\
			GLchar info[512];																		\
			glGetShaderInfoLog(shader, 512, NULL, info);											\
			std::cout << info << std::endl;															\
		}																							\
	}																								\
	while(0)																						


#define CHECK_PROGRAM(program)																		\
	do {																							\
		GLint success;																				\
		glGetProgramiv(program, GL_LINK_STATUS, &success);											\
		if (!success) {																				\
			std::cout << "Program link failed (line " << __LINE__ << "):" << std::endl;				\
			GLchar info[512];																		\
			glGetProgramInfoLog(program, 512, NULL, info);											\
			std::cout << info << std::endl;															\
		}																							\
	}																								\
	while(0)	

namespace fastui
{
	ShaderProgram::ShaderProgram() {
	}

	ShaderProgram::ShaderProgram(std::string name) : valid(true) {
		load(name);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& o)
	{
		*this = o;
		o.valid = false;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& o)
	{
		*this = o;
		o.valid = false;
		return *this;
	}

	ShaderProgram::~ShaderProgram() {
		if (valid)
			glDeleteProgram(m_shaderProgram);
	}

	void ShaderProgram::load(const std::string& name) {
		std::string vertexShaderString;
		std::string fragmentShaderString;
		
		std::ifstream vertexShaderFile(name + ".vert");
		std::ifstream fragmentShaderFile(name + ".frag");
		
		if (vertexShaderFile.is_open())
			vertexShaderString = std::string((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
		else
			std::cout << "Failed to load " << name << ".vert" << std::endl;

		if (fragmentShaderFile.is_open())
			fragmentShaderString = std::string((std::istreambuf_iterator<char>(fragmentShaderFile)), std::istreambuf_iterator<char>());
		else
			std::cout << "Failed to load " << name << ".frag" << std::endl;

		if (vertexShaderFile.is_open() && fragmentShaderFile.is_open())
			loadString(vertexShaderString, fragmentShaderString);
	}

	void ShaderProgram::loadString(const std::string& vertexShaderStr, const std::string& fragmentShaderStr) {
		const char* vertexShaderSource = vertexShaderStr.c_str();
		const char* fragmentShaderSource = fragmentShaderStr.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		CHECK_SHADER(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		CHECK_SHADER(fragmentShader);

		m_shaderProgram = glCreateProgram();
		glAttachShader(m_shaderProgram, vertexShader);
		glAttachShader(m_shaderProgram, fragmentShader);
		glLinkProgram(m_shaderProgram);
		CHECK_PROGRAM(m_shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void ShaderProgram::use() {
		glUseProgram(m_shaderProgram);
	}

	GLuint ShaderProgram::get() {
		return m_shaderProgram;
	}

	void ShaderProgram::set1f(std::string name, float value) {
		glUniform1f(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
	}

	void ShaderProgram::set3fv(std::string name, const glm::vec3& vector) {
		glUniform3fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, glm::value_ptr(vector));
	}

	void ShaderProgram::set4fv(std::string name, const glm::vec4& vector)
	{
		glUniform4fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, glm::value_ptr(vector));
	}

	void ShaderProgram::setMatrix4fv(std::string name, const glm::mat4& matrix) {
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void ShaderProgram::setMatrix4fv(std::string name, float* matrix) {
		glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_FALSE, matrix);
	}
};
