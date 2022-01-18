#include "JSONHelpers.h"

namespace JSON
{
	bool ReadVec2(glm::vec2& readTo, const nlohmann::json& j)
	{
		if (!j.contains("x") || !j.contains("y"))
			return false;

		readTo.x = j["x"];
		readTo.y = j["y"];

		return true;
	}

	bool ReadVec3(glm::vec3& readTo, const nlohmann::json& j)
	{
		if (!j.contains("x") || !j.contains("y") || !j.contains("z"))
			return false;

		readTo.x = j["x"];
		readTo.y = j["y"];
		readTo.z = j["z"];

		return true;
	}

	bool ReadVec4(glm::vec4& readTo, const nlohmann::json& j)
	{
		if (!j.contains("x") || !j.contains("y") || !j.contains("z") || !j.contains("w"))
		return false;

		readTo.x = j["x"];
		readTo.y = j["y"];
		readTo.z = j["z"];
		readTo.w = j["w"];

		return true;
	}

	bool WriteVec2(glm::vec2& readFrom, nlohmann::json& j)
	{
		if (j.is_null())
			return false;

		j["x"] = readFrom.x;
		j["y"] = readFrom.y;

		return true;
	}

	bool WriteVec3(glm::vec3& readFrom, nlohmann::json& j)
	{
		if (j.is_null())
			return false;

		j["x"] = readFrom.x;
		j["y"] = readFrom.y;
		j["z"] = readFrom.z;

		return true;
	}

	bool WriteVec4(glm::vec4& readFrom, nlohmann::json& j)
	{
		if (j.is_null())
			return false;

		j["x"] = readFrom.x;
		j["y"] = readFrom.y;
		j["z"] = readFrom.z;
		j["w"] = readFrom.w;

		return true;
	}
}