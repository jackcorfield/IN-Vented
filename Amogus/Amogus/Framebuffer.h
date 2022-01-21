#pragma once

class Framebuffer
{
public:
	Framebuffer();
	~Framebuffer();

	void Bind();
	void Unbind();

	unsigned int GetRenderTextureID();

private:
	unsigned int m_frameBufferObject;
	unsigned int m_texture;
	unsigned int m_depthRenderBuffer;
};