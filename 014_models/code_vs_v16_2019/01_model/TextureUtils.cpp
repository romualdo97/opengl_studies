#include "TextureUtils.h"

unsigned int load_texture(const char* srcToTexture, bool flipVertically)
{
	// tell stb library to flip images in load
	stbi_set_flip_vertically_on_load(flipVertically);

	// load texture data using stb_image header library
	int image_width, image_height, nrChannels;
	unsigned char* image_data = stbi_load(srcToTexture, &image_width, &image_height, &nrChannels, 0);

	// Generate OpenGL texture object
	unsigned int texture;
	glGenTextures(1, &texture);

	// bind for configuration
	glActiveTexture(GL_TEXTURE0); // activate texture unit 0
	glBindTexture(GL_TEXTURE_2D, texture);

	// put imge_data to previously created opengl texture object
	if (image_data)
	{
		if (nrChannels == 3) // jpg
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
		}
		else if (nrChannels == 4) // png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture data\n";
	}

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image_data);

	return texture;
}