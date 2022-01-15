#include "FileHandler.h"
#include "LodePNG/lodepng.h"
#include <iostream>

PNG_Data FileHandler::ReadPNG(const char* filename)
{
	std::vector<unsigned char> image;
	unsigned int width, height;
	unsigned int error = lodepng::decode(image, width, height, filename);

	if (error != 0)
	{
		std::cout << "Error reading file " << filename << std::endl;
	}
	
	PNG_Data data(image, width, height);
	data.ToRGBA();

	return data;
}

// AP - Passing by value instead of by reference because we're changing the format of the data and it'd mess stuff up if we touched the canonical stuff
void FileHandler::WritePNG(PNG_Data data, const char* filename)
{
	data.ToBGRA();
	std::vector<unsigned char> buffer;
	lodepng::encode(buffer, data.m_image, data.m_width, data.m_height);
	lodepng::save_file(buffer, filename);
}