#include "ShaderFactory.h"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

#define INVALID_SHADER_ID 0

namespace ShaderFactory
{
	void CheckShaderCompileErrors(const GLuint shaderID);
	void CheckShaderProgramLinkErrors(const GLuint programID);
	GLuint CompileShaderFromFile(const std::string& filePath, const GLint shaderType);

	Shader* CreatePipelineShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
	{
		const GLuint vertexID = CompileShaderFromFile(vertexPath, GL_VERTEX_SHADER);
		const GLuint fragmentID = CompileShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);
		const GLuint geometryID = !geometryPath.empty() ? CompileShaderFromFile(geometryPath, GL_GEOMETRY_SHADER) : INVALID_SHADER_ID;

		// Check if any shaders failed to compile
		if (vertexID == INVALID_SHADER_ID || fragmentID == INVALID_SHADER_ID || (!geometryPath.empty() && geometryID == INVALID_SHADER_ID))
		{
			return nullptr;
		}

		// Create shader program using compiled shaders
		const GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexID);
		glAttachShader(programID, fragmentID);
		if (!geometryPath.empty())
		{
			glAttachShader(programID, geometryID);
		}
		glLinkProgram(programID);
		CheckShaderProgramLinkErrors(programID);

		// Shaders are now linked into program and can be deleted
		glDetachShader(programID, vertexID);
		glDeleteShader(vertexID);
		glDetachShader(programID, fragmentID);
		glDeleteShader(fragmentID);
		if (!geometryPath.empty())
		{
			glDetachShader(programID, geometryID);
			glDeleteShader(geometryID);
		}

		return new Shader(name, programID);
	}

	ComputeShader* CreateComputeShader(const std::string& name, const std::string& computePath, const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ)
	{
		const GLuint computeID = CompileShaderFromFile(computePath, GL_COMPUTE_SHADER);

		// Check if shader failed to compile
		if (computeID == 0)
		{
			return nullptr;
		}

		// Create shader program using compiled shader
		const GLuint programID = glCreateProgram();
		glAttachShader(programID, computeID);
		glLinkProgram(programID);
		CheckShaderProgramLinkErrors(programID);

		// Shader is now linked into program and can be deleted
		glDetachShader(programID, computeID);
		glDeleteShader(computeID);

		return new ComputeShader(name, programID, numGroupsX, numGroupsY, numGroupsZ);
	}

	void CheckShaderCompileErrors(const GLuint shaderID)
	{
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			GLchar* infoLog = new GLchar[logLength];
			glGetShaderInfoLog(shaderID, logLength, &logLength, infoLog);
			std::cerr << "Error: Shader compilation failed: " << infoLog << std::endl;
			delete[] infoLog;
		}
	}

	void CheckShaderProgramLinkErrors(const GLuint programID)
	{
		GLint logLength;
		glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			GLchar* infoLog = new GLchar[logLength];
			glGetProgramInfoLog(programID, logLength, &logLength, infoLog);
			std::cerr << "Error: Shader program linking failed: " << infoLog << std::endl;
			delete[] infoLog;
		}
	}

	GLuint CompileShaderFromFile(const std::string& filePath, const GLint shaderType)
	{
		if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER && shaderType != GL_GEOMETRY_SHADER && shaderType != GL_COMPUTE_SHADER)
		{
			std::cerr << "Invalid shader type requested in CompileShaderFromFile!" << std::endl;
			return INVALID_SHADER_ID;
		}

		std::ifstream inFile(filePath);
		if (!inFile.good())
		{
			std::cerr << "Failed to open shader file at: " << filePath << std::endl;
			return INVALID_SHADER_ID;
		}

		std::stringstream shaderCodeStream;
		shaderCodeStream << inFile.rdbuf();
		std::string shaderCodeString = shaderCodeStream.str();
		const GLchar* shaderCode = shaderCodeString.c_str();

		const GLuint id = glCreateShader(shaderType);
		glShaderSource(id, 1, &shaderCode, NULL);
		glCompileShader(id);
		CheckShaderCompileErrors(id);

		inFile.close();

		return id;
	}
}