# Texture parameters

**Set wrap mode:**

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

If choosen `GL_CLAMP_TO_BORDER` we must to specify also a border color:
	
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

The possible wrap modes are:
		
> - `GL_REPEAT`: The default behavior for textures. Repeats the texture image.
- `GL_MIRRORED_REPEAT`: Same as GL_REPEAT but mirrors the image with each repeat.
- `GL_CLAMP_TO_EDGE`: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
- `GL_CLAMP_TO_BORDER`: Coordinates outside the range are now given a user-specified border color.

**Texture filtering:**

Texture filtering can be set for magnifying and minifying operations (when scaling up or downwards) so you could for example use nearest neighbor filtering when textures are scaled downwards and linear filtering for upscaled textures.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

> - `GL_NEAREST`, OpenGL selects the pixel which center is closest to the **texture coordinate**.
- `GL_LINEAR (also known as (bi)linear filtering)` The smaller the distance from the texture coordinate to a texel's center, the more that texel's color contributes to the sampled color.

**Texture Mipmap filtering:**
When object has an attached high resolution texture and is far away from camera in the scene, it will probably only produce a few fragments, OpenGL has difficulties retrieving the right color value for its fragment from the high resolution texture, since it has to pick a texture color for a fragment that spans a large part of the texture. This will produce visible artifacts on small objects, not to mention the waste of memory to use high resolution textures on small objects.

To solve this issue OpenGL uses a concept called mipmaps that is basically a collection of texture images where each subsequent texture is twice as small compared to the previous one.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

Mipmaps are only for use in minification texture operations (textures get downscaled): texture magnification doesn't use mipmaps and giving it a mipmap filtering option will generate an OpenGL `GL_INVALID_ENUM` error code.

> - `GL_NEAREST_MIPMAP_NEAREST`: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
- `GL_LINEAR_MIPMAP_NEAREST`: takes the nearest mipmap level and samples using linear interpolation.
- `GL_NEAREST_MIPMAP_LINEAR`: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples via nearest neighbor interpolation.
- `GL_LINEAR_MIPMAP_LINEAR`: linearly interpolates between the two closest mipmaps and samples the texture via linear interpolation.

# loading textures

**Step 1**
Download the header library stb_image from [here](https://github.com/nothings/stb/blob/master/stb_image.h).

**Step 2**
Paste it into your `($OPENGL_DIR)\include\` directory eg.: 
`($OPENGL_DIR)\include\stb\stb_image.h`

**Step 3**
Create a `*.cpp` file for put the stb definitions.

	// stb_image.cpp

	#define STB_IMAGE_IMPLEMENTATION
	#include <stb\stb_image.h>
	// here will be the implementation of the image loader library: stb 

**Step 4**
Include `stb_image.h` into your `main.cpp` and load texture data.

	// main.cpp
	#include <stb\stb_image.h>
	// load texture data using stb_image header library
	int i_w, i_h, nrChannels;
	unsigned char *image_data = stbi_load("wall.jpg", &i_w, &i_h, &nrChannels, 0);

# generating textures

**Step 1**
Generate opengl texture object.
	
	unsigned int texture;
	glGenTextures(1, &texture);

**Step 2**
Bind texture object to GL_TEXTURE_2D for later configuration to the currently bound texture object.

	glBindTexture(GL_TEXTURE_2D, texture);

**Step 5 (optinal)**
Set custom texture parameters

	// set the texture wrapping/filtering options (on the currently bound texture object)
	// configure wrap mode in s and t dimensions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// configure minification and magnification filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

**Step 4**
Put image data to previously created opengl texture object.

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, i_w, i_h, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glGenerateMipmap(GL_TEXTURE_2D);

> - The first argument specifies the texture target; setting this to GL_TEXTURE_2D means this operation will generate a texture on the currently bound texture object at the same target (so any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D will not be affected).
- The second argument specifies the mipmap level for which we want to create a texture for if you want to set each mipmap level manually, but we'll leave it at the base level which is 0.
- The third argument tells OpenGL in what kind of format we want to store the texture. Our image has only RGB values so we'll store the texture with RGB values as well.
- The 4th and 5th argument sets the width and height of the resulting texture. We stored those earlier when loading the image so we'll use the corresponding variables.
- The next argument should always be 0 (some legacy stuff).
- The 7th and 8th argument specify the format and datatype of the source image. We loaded the image with RGB values and stored them as chars (bytes) so we'll pass in the corresponding values.
- The last argument is the actual image data.

**Step 5**
Free memory

	stbi_image_free(image_data);