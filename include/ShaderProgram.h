#pragma once

#include <string>
#include <glm\glm.hpp>
#include <glad\glad.h>

namespace fastui
{
	class ShaderProgram {
	public:
		ShaderProgram();
		explicit ShaderProgram(std::string name);
		ShaderProgram(const ShaderProgram& sh) = delete;
		ShaderProgram(ShaderProgram&&);
		ShaderProgram& operator=(ShaderProgram&&);
		virtual ~ShaderProgram();

		void load(const std::string& name);
		void loadString(const std::string& vertexShader, const std::string& fragmentShader);

		void use();
		GLuint get();

		void set1f(std::string name, float value);
		void set3fv(std::string name, const glm::vec3& vector);
		void set4fv(std::string name, const glm::vec4& vector);
		void setMatrix4fv(std::string name, const glm::mat4& matrix);
		void setMatrix4fv(std::string name, float* matrix);

	private:
		ShaderProgram& operator=(const ShaderProgram&) = default;

		GLuint m_shaderProgram;
		bool valid = false;
	};
};
