#pragma once

#include <glm.hpp>
#include <string>

// Stores ID of a shader program and abstracts interaction with it.
class Shader
{
public:
	// Do not call outside ShaderFactory! While this constructor takes file paths, it does not compile shader files
	Shader(const std::string& name, const unsigned int programID, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
	~Shader();
	
	// Call before draw call to use this shader program.
	void Use() const;
	// Call to unbind shader program (good practice when you are done with it).
	void Unuse() const;

	void SetUniform(const std::string& name, const bool value) const;
	void SetUniform(const std::string& name, const int value) const;
	void SetUniform(const std::string& name, const unsigned int value) const;
	void SetUniform(const std::string& name, const float value) const;
	void SetUniform(const std::string& name, const float x, const float y) const; // vec2
	void SetUniform(const std::string& name, const glm::vec2& value) const;
	void SetUniform(const std::string& name, const float x, const float y, const float z) const; // vec3
	void SetUniform(const std::string& name, const glm::vec3& value) const;
	void SetUniform(const std::string& name, const float x, const float y, const float z, const float w) const; // vec4
	void SetUniform(const std::string& name, const glm::vec4& value) const;
	void SetUniform(const std::string& name, const glm::mat3& value) const;
	void SetUniform(const std::string& name, const glm::mat4& value) const;

	const std::string m_name;
	const std::string m_vertexPath, m_fragmentPath, m_geometryPath;

private:
	// Not copyable
	Shader(const Shader&) = delete;
	Shader& operator=(Shader&) = delete;

	const unsigned int m_id;
};

// Adds threading size data to base Shader, as well as a dispatching function.
class ComputeShader :
	public Shader
{
public:
	// Do not call outside ShaderFactory! While this constructor takes a file path, it does not compile shader files
	ComputeShader(const std::string& name, const unsigned int programID, const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ, const std::string& computePath);

	// Call to dispatch this shader (replaces the draw call). You still need to call Use() before this.
	void Dispatch() const;

	inline void SetNumGroups(const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ = 1) { m_numGroupsX = numGroupsX; m_numGroupsY = numGroupsY; m_numGroupsZ = numGroupsZ; }

	inline std::string GetComputePath() const { return m_computePath; }

private:
	unsigned int m_numGroupsX, m_numGroupsY, m_numGroupsZ;

	const std::string m_computePath;
};