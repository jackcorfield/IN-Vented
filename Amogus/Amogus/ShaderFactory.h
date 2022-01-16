#pragma once

#include <string>
#include "Shader.h"

// Call these to compile and link shader programs
namespace ShaderFactory
{
	// Use to create a regular vertex & fragment shader. Optionally creates a geometry shader too.
	Shader* CreatePipelineShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");

	// The numGroups params define how many groups to separate the image into when dispatched, used for parallel processing.
	ComputeShader* CreateComputeShader(const std::string& name, const std::string& computePath, const unsigned int numGroupsX, const unsigned int numGroupsY, const unsigned int numGroupsZ = 1);
}