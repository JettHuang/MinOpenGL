#ifndef __TGA_H__
#define __TGA_H__

#include <stdio.h>										// Standard I/O header 
#include "../include/GL/gl.h"
#include "texture.h"



typedef struct
{
	GLubyte Header[12];									// TGA File Header
} TGAHeader;


typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;


TGAHeader tgaheader;									// TGA header
TGA tga;												// TGA image data

GLboolean LoadTGA(Texture * texture, char * filename);
GLboolean LoadUncompressedTGA(Texture *, char *, FILE *);	// Load an Uncompressed file
GLboolean LoadCompressedTGA(Texture *, char *, FILE *);		// Load a Compressed file

#endif






