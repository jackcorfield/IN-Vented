#pragma once

#include <vector>

struct PNG_Data
{
	PNG_Data(std::vector<unsigned char> image, unsigned int width, unsigned int height) : m_image(image), m_width(width), m_height(height) {}

	// The format we want when we import
	void ToRGBA()
	{
		std::vector<unsigned char> buffer(m_image.size());

		for (unsigned int y = 0; y < m_height; y++)
		{
			for (unsigned int x = 0; x < m_width; x++)
			{
				int oldPos = (m_height - y - 1) * (m_width * 4) + 4 * x;
				int newPos = y * (m_width * 4) + 4 * x;

				// BGRA -> RGBA
				buffer[newPos+0] = m_image[oldPos+2]; // R(0) = R(2)
				buffer[newPos+1] = m_image[oldPos+1]; // G(1) = G(1)
				buffer[newPos+2] = m_image[oldPos+0]; // B(2) = B(0)
				buffer[newPos+3] = m_image[oldPos+3]; // A(3) = A(3)
			}
		}

		m_image = buffer;
	}

	// The format we want when we export
	void ToBGRA()
	{
		std::vector<unsigned char> buffer(m_image.size());

		for (unsigned int y = 0; y < m_height; y++)
		{
			for (unsigned int x = 0; x < m_width; x++)
			{
				int oldPos = (m_height - y - 1) * (m_width * 4) + 4 * x;
				int newPos = y * (m_width * 4) + 4 * x;

				// RGBA -> BGRA
				buffer[newPos + 2] = m_image[oldPos + 0]; // R(2) = R(0)
				buffer[newPos + 1] = m_image[oldPos + 1]; // G(1) = G(1)
				buffer[newPos + 0] = m_image[oldPos + 2]; // B(0) = B(2)
				buffer[newPos + 3] = m_image[oldPos + 3]; // A(3) = A(3)
			}
		}

		m_image = buffer;
	}

	std::vector<unsigned char> m_image;
	unsigned int m_width;
	unsigned int m_height;
};

class FileHandler
{
public:
	static PNG_Data ReadPNG(const char* filename);
	static void WritePNG(PNG_Data data, const char* filename);

	/*
	AP - If the code is working, then hi2 should match hi identically with no issues
	static void TestPNG() 
	{
		PNG_Data data = FileHandler::ReadPNG("hi.png");
		FileHandler::WritePNG(data, "hi2.png");
	}
	*/
};