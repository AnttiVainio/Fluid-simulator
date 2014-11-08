/** texture.cpp **/

#include <GL/gl.h>
#include <cmath>

//This is the resolution for the fluid texture
#define TEX_SIZE 128

//Function for creating an empty OpenGL texture
GLuint create_empty_texture() {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return texture;
}

//This generates the texture we use for the fluid
//The texture is basically a circular sprite that fades towards the edges
GLuint create_water_texture() {
	//Create the pixels is rgba format
	unsigned char *pixels =  new unsigned char[4 * TEX_SIZE * TEX_SIZE];
	for(unsigned short i = 0; i < TEX_SIZE; i++) {
		for(unsigned short j = 0; j < TEX_SIZE; j++) {
			const unsigned int pos = (i * TEX_SIZE + j) * 4; //position in the array
			//value is the brightness of the pixels
			//The value is clamped to [0, 255] so it can be used directly as a color value
			const float value = 16.0
				/ sqrt( ((float)i - (float)TEX_SIZE / 2.0) * ((float)i - (float)TEX_SIZE / 2.0) + ((float)j - (float)TEX_SIZE / 2.0) * ((float)j - (float)TEX_SIZE / 2.0) )
				* TEX_SIZE - 32.0;
			//Every color (even alpha) uses the value
			pixels[pos] = value < 0 ? 0 : (value >= 256.0 ? 255 : (unsigned char)value);
			pixels[pos + 1] = pixels[pos];
			pixels[pos + 2] = pixels[pos];
			pixels[pos + 3] = pixels[pos];
		}
	}
	//Create the actual texture
	GLuint texture = create_empty_texture();
	glTexImage2D(GL_TEXTURE_2D, 0, 4, TEX_SIZE, TEX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	delete [] pixels;
	return texture;
}
