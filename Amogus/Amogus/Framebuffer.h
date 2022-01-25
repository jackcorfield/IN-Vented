#pragma once

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Bind();
	void Unbind();
	void Resize(const int width, const int height);

	unsigned int GetRenderTextureID();

private:
	unsigned int m_frameBufferObject;
	unsigned int m_texture;
	unsigned int m_depthRenderBuffer;
};