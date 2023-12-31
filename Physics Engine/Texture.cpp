
#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION



Texture::Texture()
{
	/*this->path = NULL;
	this->textureId = NULL;*/
}

Texture::Texture(const char* filePath)
{
	this->path = filePath;
	this->textureId = loadTexture(filePath);
}



unsigned int Texture:: loadTexture(char const* path)
{
	unsigned int textureID = 0;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	std::cout << "LoadedTexture successfully : " << path << std::endl;
	return textureID;
}

void  Texture::LoadTexture(const char* filePath, const char* _type)
{
	this->path = filePath;
	this->type = _type;
	this->textureId = loadTexture(filePath);
}