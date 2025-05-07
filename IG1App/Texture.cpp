#include "Texture.h"

#include "Image.h"

Texture::~Texture()
{
	if (mId != 0)
		glDeleteTextures(1, &mId);
}

void
Texture::init()
{
	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);

	// Filters and clamping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     // GL_CLAMP GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);     // GL_CLAMP
}

void
Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, mId);
}

void
Texture::load(const std::string& name, GLubyte alpha)
{
	if (mId == 0)
		init();

	Image image;
	image.load(name);

	if (alpha != 255)
		image.setAlpha(alpha);

	mWidth = image.width();
	mHeight = image.height();

	GLint level = 0;  // Base image level
	GLint border = 0; // No border

	glBindTexture(GL_TEXTURE_2D, mId);
	glTexImage2D(GL_TEXTURE_2D,
	             level,
	             GL_RGBA,
	             mWidth,
	             mHeight,
	             border,
	             GL_RGBA,
	             GL_UNSIGNED_BYTE,
	             image.data());

	glBindTexture(GL_TEXTURE_2D, 0);
}

void
Texture::setWrap(GLuint wp) // GL_REPEAT, GL_CLAMP_TO_EDGE, ...
{
	glBindTexture(GL_TEXTURE_2D, mId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wp);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Ap-34, carga como textura una ventana de la escena con simensiones width x height
void
Texture::loadColorBuffer(GLsizei width, GLsizei height, GLuint buffer) 
{
	if (mId == 0)
		init();


	mWidth = width;
	mHeight = height;

	GLint level = 0;  // Base image level
	GLint border = 0; // No border

	glReadBuffer(buffer); // por defecto GL_BACK

	glBindTexture(GL_TEXTURE_2D, mId);

	glCopyTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, 0, 0, width, height, border);

	
	glBindTexture(GL_TEXTURE_2D, 0);
}

//Ap-36, descarga de la imagen
void 
Texture::download(const std::string& filename)
{
	GLint level = 0;  // Base image level

// guardar pixeles en formato  glm::u8vec4;
	glm::u8vec4* pixels = new glm::u8vec4[mWidth * mHeight];
	glBindTexture(GL_TEXTURE_2D, mId);

	glGetTexImage(GL_TEXTURE_2D, level, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
	// Voltear la imagen en el eje Y (reflejarla verticalmente)
	for (GLsizei y = 0; y < mHeight / 2; ++y) {
		for (GLsizei x = 0; x < mWidth; ++x) {
			std::swap(pixels[y * mWidth + x], pixels[(mHeight - 1 - y) * mWidth + x]);
		}
	}

	// download image
	Image image;
	image.load(pixels, mWidth, mHeight);
	image.save(filename);
	delete [] pixels;
}