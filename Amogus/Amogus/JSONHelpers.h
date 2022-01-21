#pragma once

#include <glm.hpp>
#include <nlohmann/json.hpp>

namespace JSON
{
	template <typename T>
	inline bool Read(T& readTo, const nlohmann::json& j)
	{
		if (j.is_null())
			return false;

		readTo = j;

		return true;
	}

	inline bool ReadVec2(glm::vec2& readTo, const nlohmann::json& j)
	{
		if (!j.contains("x") || !j.contains("y"))
			return false;

		readTo.x = j["x"];
		readTo.y = j["y"];

		return true;
	}

	inline bool ReadVec3(glm::vec3& readTo, const nlohmann::json& j)
	{
		if (!j.contains("x") || !j.contains("y") || !j.contains("z"))
			return false;

		readTo.x = j["x"];
		readTo.y = j["y"];
		readTo.z = j["z"];

		return true;
	}

	inline bool ReadVec4(glm::vec4& readTo, const nlohmann::json& j)
	{
		if (!j.contains("x") || !j.contains("y") || !j.contains("z") || !j.contains("w"))
			return false;

		readTo.x = j["x"];
		readTo.y = j["y"];
		readTo.z = j["z"];
		readTo.w = j["w"];

		return true;
	}

	template <typename T>
	inline bool Write(const T& readFrom, nlohmann::json& j)
	{
		//if (j.is_null())
		//	return false;

		j = readFrom;

		return true;
	}

	inline bool WriteVec2(const glm::vec2& readFrom, nlohmann::json& j)
	{
		//if (j.is_null())
		//	return false;

		j["x"] = readFrom.x;
		j["y"] = readFrom.y;

		return true;
	}

	inline bool WriteVec3(const glm::vec3& readFrom, nlohmann::json& j)
	{
		//if (j.is_null())
		//	return false;

		j["x"] = readFrom.x;
		j["y"] = readFrom.y;
		j["z"] = readFrom.z;

		return true;
	}

	inline bool WriteVec4(const glm::vec4& readFrom, nlohmann::json& j)
	{
		//if (j.is_null())
		//	return false;

		j["x"] = readFrom.x;
		j["y"] = readFrom.y;
		j["z"] = readFrom.z;
		j["w"] = readFrom.w;

		return true;
	}
}