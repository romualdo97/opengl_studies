#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <iostream>

unsigned int load_texture(const char* srcToTexture, bool flipVertically = true);