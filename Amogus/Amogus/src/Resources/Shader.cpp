#include "Shader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& name, const unsigned int programID, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) :
	m_name(name),
	m_id(programID),
	m_vertexPath(vertexPath),
	m_fragmentPath(fragmentPath),
	m_geometryPath(geometryPath)
{}

Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void Shader::Use() const
{
	glUseProgram(m_id);
}

void Shader::Unuse() const
{
	glUseProgram(0);
}

void Shader::SetUniform(const std::string& name, const bool value) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform1i(location, value);
}

void Shader::SetUniform(const std::string& name, const int value) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform1i(location, value);
}

void Shader::SetUniform(const std::string& name, const unsigned int value) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform1ui(location, value);
}

void Shader::SetUniform(const std::string& name, const float value) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform1f(location, value);
}

void Shader::SetUniform(const std::string& name, const float x, const float y) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform2f(location, x, y);
}

void Shader::SetUniform(const std::string& name, const glm::vec2& value) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform2f(location, value.x, value.y);
}

void Shader::SetUniform(const std::string& name, const float x, const float y, const float z) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform3f(location, x, y, z);
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetUniform(const std::string& name, const float x, const float y, const float z, const float w) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform4f(location, x, y, z, w);
}

void Shader::SetUniform(const std::string& name, const glm::vec4& value) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(const std::string& name, const glm::mat3& value) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniform(const std::string& name, const glm::mat4& value) const
{
	GLint location = glGetUniformLocation(m_id, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

ComputeShader::ComputeShader(const std::string& name, const unsigned int programID, const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ, const std::string& computePath) :
	Shader(name, programID, "", "", ""),
	m_numGroupsX(numGroupsX == 0 ? 1 : numGroupsX),
	m_numGroupsY(numGroupsY == 0 ? 1 : numGroupsY),
	m_numGroupsZ(numGroupsZ == 0 ? 1 : numGroupsZ),
	m_computePath(computePath)
{}

void ComputeShader::Dispatch() const
{
	glDispatchCompute(m_numGroupsX, m_numGroupsY, m_numGroupsZ);
}