#pragma once

#include <glm/glm.hpp>
#include <nlohmann/include/nlohmann/json.hpp>

namespace JSON
{
	template <typename T>
	inline bool Read(T& readTo, const nlohmann::json& j, const std::string& field)
	{
		if (j.is_null() || !j.contains(field))
			return false;

		readTo = j[field];

		return true;
	}

	inline bool ReadVec2(glm::vec2& readTo, const nlohmann::json& j, const std::string& field)
	{
		if (!j.contains(field))
			return false;

		if (!j[field].contains("x") || !j[field].contains("y"))
			return false;

		readTo.x = j[field]["x"];
		readTo.y = j[field]["y"];

		return true;
	}

	inline bool ReadVec3(glm::vec3& readTo, const nlohmann::json& j, const std::string& field)
	{
		if (!j.contains(field))
			return false;

		if (!j[field].contains("x") || !j[field].contains("y") || !j[field].contains("z"))
			return false;

		readTo.x = j[field]["x"];
		readTo.y = j[field]["y"];
		readTo.z = j[field]["z"];

		return true;
	}

	inline bool ReadVec4(glm::vec4& readTo, const nlohmann::json& j, const std::string& field)
	{
		if (!j.contains(field))
			return false;

		if (!j[field].contains("x") || !j[field].contains("y") || !j[field].contains("z") || !j[field].contains("w"))
			return false;

		readTo.x = j[field]["x"];
		readTo.y = j[field]["y"];
		readTo.z = j[field]["z"];
		readTo.w = j[field]["w"];

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