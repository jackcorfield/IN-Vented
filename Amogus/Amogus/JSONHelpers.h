#pragma once

#include <glm.hpp>
#include <nlohmann/json.hpp>

namespace JSON
{
	template <typename T>
	static bool Read(T& readTo, const nlohmann::json& j)
	{
		if (j.is_null())
			return false;

		readTo = j;

		return true;
	}

	bool ReadVec2(glm::vec2& readTo, const nlohmann::json& j);
	bool ReadVec3(glm::vec3& readTo, const nlohmann::json& j);
	bool ReadVec4(glm::vec4& readTo, const nlohmann::json& j);

	template <typename T>
	static bool Write(const T& readFrom, nlohmann::json& j)
	{
		if (j.is_null())
			return false;

		j = readFrom;

		return true;
	}

	bool WriteVec2(const glm::vec2& readFrom, nlohmann::json& j);
	bool WriteVec3(const glm::vec3& readFrom, nlohmann::json& j);
	bool WriteVec4(const glm::vec4& readFrom, nlohmann::json& j);
}