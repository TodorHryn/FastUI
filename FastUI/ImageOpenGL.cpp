#include "ImageOpenGL.h"
#include "Util.h"

#define STB_IMAGE_IMPLEMENTATION   
#include <stb_image.h>

ImageOpenGL::ImageOpenGL()
	: m_width(0)
	, m_height(0)
{

}

ImageOpenGL::~ImageOpenGL() {
	
}

void ImageOpenGL::load(int resourceId) {
	std::vector<uint8_t> data;
	LoadResource(resourceId, data);

	int width, height, channels;
	uint8_t* img = stbi_load_from_memory(data.data(), data.size(), &width, &height, &channels, 3);
	m_width = width;
	m_height = height;
	load(img);
}

void ImageOpenGL::load(const std::string& path) {
	int width, height, channels;
	uint8_t* img = stbi_load(path.c_str(), &width, &height, &channels, 3);
	m_width = width;
	m_height = height;
	load(img);
}

void ImageOpenGL::load(uint8_t* glData) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		m_width,
		m_height,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		glData
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
