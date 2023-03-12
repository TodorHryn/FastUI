#pragma once
#include <cstdint>
#include <glad\glad.h>
#include "Drawer.h"

struct ImageOpenGL : public Drawer::Image
{
	ImageOpenGL();
	virtual ~ImageOpenGL();

	void load(int resourceId);
	void load(const std::string& path);

	int32_t m_width, m_height;
	GLuint m_texture;

private:
	void load(uint8_t* glData);
};
