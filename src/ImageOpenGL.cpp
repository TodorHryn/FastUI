#include "ImageOpenGL.h"
#include "Util.h"

#define STB_IMAGE_IMPLEMENTATION   
#include <stb_image.h>

namespace fastui
{
	ImageOpenGL::ImageOpenGL()
		: m_width(0)
		, m_height(0)
		, m_texture(-1)
	{

	}

	ImageOpenGL::ImageOpenGL(ImageOpenGL&& o) noexcept
	{
		*this = o;
		o.valid = false;
	}

	ImageOpenGL& ImageOpenGL::operator=(ImageOpenGL&& o) noexcept
	{
		*this = o;
		o.valid = false;
		return *this;
	}

	ImageOpenGL::~ImageOpenGL() {
		if (valid)
			glDeleteTextures(1, &m_texture);
	}

	void ImageOpenGL::load(const std::string& path) {
		int width, height, channels;
		uint8_t* img = stbi_load(path.c_str(), &width, &height, &channels, 3);
		if (img)
		{
			m_width = width;
			m_height = height;
			valid = true;
			load(img);
		}
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
};
