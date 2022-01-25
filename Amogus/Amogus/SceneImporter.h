#pragma once

#include <string>

namespace SceneImporter
{
	bool ImportSceneFromFile(const std::string& filePath, bool setToActive = false);
};